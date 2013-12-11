/**
 * Linux only kosher driver for CTRV module
 * Julian Lewis Mon 9 Dec 2013 BE/CO/HT
 *
 * This driver has been seriously modified to remove a lot of legacy code
 * that was inherited from the LynxOs version.
 *
 * Whats changed
 *    All LynxOs stuff is now suppressed.
 *    The device model now uses a single node in /dev
 *    The interrupt mechanism is based on wait_queues
 *    Standard modpars are used
 *    The PLX9030 programming no longer swaps big-little endian on BAR2
 *    All hardware access is via the io read/write macros
 *    No more handling of JTAG FPGA programming  (use my jtag prog instead)
 *    No more flash eprom handling (use my plxprog instaed)
 *    No more local BAR0 raw io
 *    No more access to PCI configuration space
 *    Many bug corrections
 */

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/types.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/sched.h>

#include <linux/pci.h>

#include <ctrhard.h>   /* Hardware description */
#include <ctrdrvr.h>   /* Public driver interface */
#include <ctrdrvrP.h>  /* Private driver structures */
#include <ctrhard.h>   /* Hardware layout */
#include <hptdc.h>     /* High prescision time to digital convertor */

#include <plx9030.h>   /* PLX9030 Registers and definition   */

#define OK 0
#define SYSERR (-1)

static int ctr_major = 0;
static char *ctr_major_name = "ctrp";

MODULE_AUTHOR("Julian Lewis BE/CO/HT CERN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Timing Receiver");
MODULE_SUPPORTED_DEVICE("CTRP");

#define MAX_DEVS CtrDrvrMODULE_CONTEXTS
#define MIN_DEV 1                         /** First valid LUN number */
#define LAST_DEV (MAX_DEVS - 1 + MIN_DEV) /** Last valid LUN number */

static int luns[MAX_DEVS];
static int pci_buses[MAX_DEVS];
static int pci_slots[MAX_DEVS];

static int luns_num;
static int pci_bus_num;
static int pci_slot_num;

module_param_array(luns,      int, &luns_num,     0);
module_param_array(pci_buses, int, &pci_bus_num,  0);
module_param_array(pci_slots, int, &pci_slot_num, 0);

MODULE_PARM_DESC(luns,      "logical unit numbers");
MODULE_PARM_DESC(pci_buses, "pci bus numbers");
MODULE_PARM_DESC(pci_slots, "pci slot numbers");

#define NAME_LEN 16
#define BARS 6         /* 0..5 but bar=0 isn't used */
#define BAR_MASK 0x3E  /* 5 bits 111110 bar 1..5 */

typedef struct {
	char name[NAME_LEN];     /* Device name */
	int vid;                 /* Vendor ID */
	int did;                 /* Device ID */
	int lun;                 /* Logical unit number */
	int pci_bus_num;         /* PCI bus number */
	int pci_slot_num;        /* PCI slot number */
	int bars;                /* Bit mask of used bars */
	void __iomem *map[BARS]; /* Corresponding memory maps */
	struct pci_dev *dev;     /* PCI device */
} mod_par_t;                     /* Module parameter block */

static mod_par_t mods[MAX_DEVS]; /* An array of MAX_DEVS module parameter blocks */

static int iluns = 0;            /* Installed luns so far */

static CtrDrvrWorkingArea Wa;
static DEFINE_MUTEX(ctr_drvr_mutex); /* Driver exclusive mutex */

/**
 * =========================================================
 * @brief Debug routine to print mod par arguments
 */

static void print_args(char *name)
{
	int i;

	printk("%s:Installation module parameters ...\n",name);
	for (i=0; i<luns_num; i++) {
		printk("%s:%02d - [Lun:%02d Bus:0x%X Slot:0x%X]\n",
		       name,i,luns[i],pci_buses[i],pci_slots[i]);
	}
	printk("%s:Declared:%d Luns\n",name,luns_num);
}

/**
 * =========================================================
 * @brief Validate insmod args
 * @return 1=OK 0=Bad
 */

static int check_args(char *name)
{
	int i, lun;

	print_args(name);

	if ((luns_num < 1)
	||  (luns_num > MAX_DEVS)) {
		printk("%s:Lun count:%d, not in:[1..%d]\n",
		       name,
		       luns_num,
		       MAX_DEVS);
		return 0;
	}
	if ((luns_num != pci_slot_num)
	||  (luns_num != pci_bus_num)) {
		printk("%s:Bad PCI parameter count:(Slots:%d,Busses:%d) should be:%d\n",
		       name,
		       pci_slot_num,
		       pci_bus_num,
		       luns_num);
		return 0;
	}
	for (i=0; i<luns_num; i++) {
		lun = luns[i];
		if ((lun < MIN_DEV) || (lun > LAST_DEV)) {
			printk("%s:Lun:%d out of [%d..%d]\n",
			       name,
			       lun,
			       MIN_DEV,
			       LAST_DEV);
			return 0;
		}
	}
	return 1;
}

/**
 * =========================================================
 * @brief Hunt for a lun number given the PCI bus and slot numbers
 * @param bus   PCI_bus number
 * @param slot  Slot number on PCI bus
 * @return -1=NOT Found else lun number
 */

static int hunt_lun(int bus, int slot)
{
	int i;
	for (i=0; i<luns_num; i++)
		if ((pci_slots[i] == slot)
		&&  (pci_buses[i] == bus))
			return luns[i];
	return -1;
}

/**
 * =========================================================
 * @brief           Get an unused Lun number
 * @return          A lun number or zero if none available
 */

static int used_luns = 0;

static int get_unused_lun(void)
{

	int i, lun, bit;

	/* Fill in declared luns if first time */

	if ((iluns) && (!used_luns)) {
		for (i=0; i<luns_num; i++) {
			lun = luns[i];
			bit = 1 << lun;
			used_luns |= bit;
		}
	}

	/* Find an unused = undeclared lun */

	for (i=MIN_DEV; i<=LAST_DEV; i++) {
		bit = 1 << i;
		if (!(used_luns & bit)) {
			used_luns |= bit;
			return i;
		}
	}
	return 0;
}

/**
 * =========================================================
 * @brief           Add the next PCI device
 * @param  pcur     Previous added device or NULL
 * @param  mpar     Module parameters
 * @return          Pointer to pci_device structure or NULL
 *
 * This routine gets the next ctrp and looks up its bus/slot
 * using hunt_lun to see if its in the supplied modpars.
 * If declared, then an entry for the given lun is made in the
 * modules table. Otherwise and unused lun guaranteed not to
 * conflict with the supplied parameters is made.
 * This means that the modpars can just be ommitted in the
 * case of a single ctrp. This functinality is very important
 * because in general no one knows how bus/slot get mapped on
 * a PC platform. So when only one ctrp/i is being installed
 * (the usual case) then simply omit the modpars in the
 * installation script. This default mechanism is widley used!!
 *
 * pci_get_device replaces the depricated find_device call
 *
 */

static struct pci_dev *add_next_dev(struct pci_dev *pcur,
				    mod_par_t *mpar)
{
	struct pci_dev *pprev = pcur;
	int cc, lun, bar, len;
	char bar_name[32];

	mpar->dev = NULL;

	pcur = pci_get_device(mpar->vid, mpar->did, pprev);
	if (!pcur)
		return NULL;

	mpar->pci_bus_num  = pcur->bus->number;
	mpar->pci_slot_num = PCI_SLOT(pcur->devfn);

	lun = hunt_lun(mpar->pci_bus_num,mpar->pci_slot_num);
	if (lun < 0) {
		printk("%s:Found undeclared module at Bus:%d Slot:%d\n",
		       mpar->name,
		       mpar->pci_bus_num,
		       mpar->pci_slot_num);

		lun = get_unused_lun();
		if (!lun)
			return NULL;

		printk("%s:Auto assigned Bus:%d Slot:%d to Lun:%d\n",
		       mpar->name,
		       mpar->pci_bus_num,
		       mpar->pci_slot_num,
		       lun);
	}
	mpar->lun = lun;

	cc = pci_enable_device(pcur);
	printk("%s:Vid:0x%X Did:0x%X Bus:%d Slot:%d Lun:%d",
	       mpar->name,
	       mpar->vid,
	       mpar->did,
	       mpar->pci_bus_num,
	       mpar->pci_slot_num,
	       mpar->lun);
	if (cc) {
		printk(" pci_enable:ERROR:%d",
		       cc);
		return NULL;
	} else
		printk(" Enabled:OK\n");

	/*
	 * Map BARS
	 */

	for (bar=0; bar<BARS; bar++) {
		if (mpar->bars & (1<<bar)) {
			sprintf(bar_name,"%s-bar:%d.lun:%d",
				mpar->name,
				bar,
				mpar->lun);

			len = pci_resource_len(pcur, bar);
			cc = pci_request_region(pcur, bar, bar_name);
			if (cc) {
				pci_disable_device(pcur);
				printk("%s:pci_request_region:len:0x%x:%s:ERROR:%d\n",
				       mpar->name,
				       len,
				       bar_name,
				       cc);
				return NULL;
			}
			printk("%s:Mapped bar:%s:OK\n",
			       mpar->name,
			       bar_name);
			mpar->map[bar] = pci_iomap(pcur,bar,len);
		}
	}
	iluns++;
	mpar->dev = pcur;
	return pcur;
}

/**
 * =========================================================
 * @brief Initialize module parameters array
 * @param name  name of module
 * @param vid   vendor id
 * @param did   device id
 * @return      number of modules initialized
 */

static int init_mod_pars(char *name, int vid, int did, int bars)
{
	int i;
	struct pci_dev *pcur = NULL;
	mod_par_t *par = NULL;

	for (i=0; i<MAX_DEVS; i++) {
		par = &mods[i];
		strncpy(par->name,name,NAME_LEN);
		par->vid  = vid;
		par->did  = did;
		par->bars = bars;
		pcur = add_next_dev(pcur,par);
		if (!pcur)
			break;
	}

	printk("%s:Initialized:%d modules\n",name,iluns);

	if (iluns != luns_num)
		printk("%s:init_mod_pars:declared:%d installed:%d Luns\n",name,luns_num,iluns);

	return iluns;
}

/*========================================================================*/
/* 32 Bit int access for CTR structure to write to hardware               */
/*========================================================================*/

static void Io32Write(uint32_t *dst, /* Hardware destination address */
		      uint32_t *src,
		      uint32_t size)
{
	int i, sb;

	sb = size/sizeof(uint32_t);
	for (i=0; i<sb; i++)
		iowrite32be(src[i],(void *) &(dst[i]));
}

/*========================================================================*/
/* 32 Bit int access for CTR structure to read from hardware             */
/*========================================================================*/

static void Io32Read(uint32_t *dst,
		     uint32_t *src, /* Hardware Source address */
		     uint32_t size)
{
	int i, sb;

	sb = size/sizeof(uint32_t);
	for (i=0; i<sb; i++)
		dst[i] = ioread32be((void *) &(src[i]));
}

/*========================================================================*/
/* 32 Bit int access copy                                                 */
/*========================================================================*/

static void Int32Copy(uint32_t *dst,
		      uint32_t *src,
		      uint32_t size)
{
	int i, sb;

	sb = size/sizeof(uint32_t);
	for (i=0; i<sb; i++)
		dst[i] = src[i];
}

/*========================================================================*/
/* Swap words                                                             */
/*========================================================================*/

static void SwapWords(uint32_t *dst,
		      uint32_t size)
{
	int i, sb;
	uint32_t lft, rgt;

	sb = size/sizeof(uint32_t);
	for (i=0; i<sb; i++) {
		rgt = lft = dst[i];
		lft = (lft & 0xFFFF0000) >> 16;
		rgt = (rgt & 0x0000FFFF);
		dst[i] = (rgt<<16) | lft;
	}
}

/* ============================== */
/* To prevent accidents only      */
/* ============================== */

static void clr_uplock(CtrDrvrModuleContext *mcon, uint32_t pw)
{
	uint32_t ck = 0;
	int i;

	for (i=0; i<8; i++) {
		ck += (pw & 0xF);
		pw = pw >> 4;
	}
	if (ck == 0x0024)
		mcon->UpLock = 0;
}

/* =========================================================================================== */
/* Print Debug message                                                                         */
/* =========================================================================================== */

struct ioctl_names_s {
	uint32_t number;        /* Ioctl number 0..N */
	char     *name;         /* Ioctl name */
};

static struct ioctl_names_s ioctl_names[CtrDrvrLAST_IOCTL] = {

{ CtrDrvrSET_SW_DEBUG,           "SET_SW_DEBUG" },           /* Set driver debug mode */
{ CtrDrvrGET_SW_DEBUG,           "GET_SW_DEBUG" },           /* Get driver debug mode */
{ CtrDrvrGET_VERSION,            "GET_VERSION" },            /* Get version date */
{ CtrDrvrSET_TIMEOUT,            "SET_TIMEOUT" },            /* Set the read timeout value */
{ CtrDrvrGET_TIMEOUT,            "GET_TIMEOUT" },            /* Get the read timeout value */
{ CtrDrvrSET_QUEUE_FLAG,         "SET_QUEUE_FLAG" },         /* Set queuing capabiulities on off */
{ CtrDrvrGET_QUEUE_FLAG,         "GET_QUEUE_FLAG" },         /* 1=Q_off 0=Q_on */
{ CtrDrvrGET_QUEUE_SIZE,         "GET_QUEUE_SIZE" },         /* Number of events on queue */
{ CtrDrvrGET_QUEUE_OVERFLOW,     "GET_QUEUE_OVERFLOW" },     /* Number of missed events */
{ CtrDrvrGET_MODULE_DESCRIPTOR,  "GET_MODULE_DESCRIPTOR" },  /* Get the current Module descriptor */
{ CtrDrvrSET_MODULE,             "SET_MODULE" },             /* Select the module to work with */
{ CtrDrvrGET_MODULE,             "GET_MODULE" },             /* Which module am I working with */
{ CtrDrvrGET_MODULE_COUNT,       "GET_MODULE_COUNT" },       /* The number of installed modules */
{ CtrDrvrRESET,                  "RESET" },                  /* Reset the module, re-establish connections */
{ CtrDrvrENABLE,                 "ENABLE" },                 /* Enable CTR module event reception */
{ CtrDrvrGET_STATUS,             "GET_STATUS" },             /* Read module status */
{ CtrDrvrGET_INPUT_DELAY,        "GET_INPUT_DELAY" },        /* Get input delay in 25ns ticks */
{ CtrDrvrSET_INPUT_DELAY,        "SET_INPUT_DELAY" },        /* Set input delay in 25ns ticks */
{ CtrDrvrGET_CLIENT_LIST,        "GET_CLIENT_LIST" },        /* Get the list of driver clients */
{ CtrDrvrCONNECT,                "CONNECT" },                /* Connect to an object interrupt */
{ CtrDrvrDISCONNECT,             "DISCONNECT" },             /* Disconnect from an object interrupt */
{ CtrDrvrGET_CLIENT_CONNECTIONS, "GET_CLIENT_CONNECTIONS" }, /* Get the list of a client connections on module */
{ CtrDrvrSET_UTC,                "SET_UTC" },                /* Set Universal Coordinated Time for next PPS tick */
{ CtrDrvrGET_UTC,                "GET_UTC" },                /* Latch and read the current UTC time */
{ CtrDrvrGET_CABLE_ID,           "GET_CABLE_ID" },           /* Cables telegram ID */
{ CtrDrvrGET_ACTION,             "GET_ACTION" },             /* Low level direct access to CTR RAM tables */
{ CtrDrvrSET_ACTION,             "SET_ACTION" },             /* Set may not modify the bus interrupt settings */
{ CtrDrvrCREATE_CTIM_OBJECT,     "CREATE_CTIM_OBJECT" },     /* Create a new CTIM timing object */
{ CtrDrvrDESTROY_CTIM_OBJECT,    "DESTROY_CTIM_OBJECT" },    /* Destroy a CTIM timing object */
{ CtrDrvrLIST_CTIM_OBJECTS,      "LIST_CTIM_OBJECTS" },      /* Returns a list of created CTIM objects */
{ CtrDrvrCHANGE_CTIM_FRAME,      "CHANGE_CTIM_FRAME" },      /* Change the frame of an existing CTIM object */
{ CtrDrvrCREATE_PTIM_OBJECT,     "CREATE_PTIM_OBJECT" },     /* Create a new PTIM timing object */
{ CtrDrvrDESTROY_PTIM_OBJECT,    "DESTROY_PTIM_OBJECT" },    /* Destroy a PTIM timing object */
{ CtrDrvrLIST_PTIM_OBJECTS,      "LIST_PTIM_OBJECTS" },      /* List PTIM timing objects */
{ CtrDrvrGET_PTIM_BINDING,       "GET_PTIM_BINDING" },       /* Search for a PTIM object binding */
{ CtrDrvrGET_OUT_MASK,           "GET_OUT_MASK" },           /* Counter output routing mask */
{ CtrDrvrSET_OUT_MASK,           "SET_OUT_MASK" },           /* Counter output routing mask */
{ CtrDrvrGET_COUNTER_HISTORY,    "GET_COUNTER_HISTORY" },    /* One deep history of counter */
{ CtrDrvrGET_REMOTE,             "GET_REMOTE" },             /* Counter Remote/Local status */
{ CtrDrvrSET_REMOTE,             "SET_REMOTE" },             /* Counter Remote/Local status */
{ CtrDrvrREMOTE,                 "REMOTE" },                 /* Remote control counter */
{ CtrDrvrGET_CONFIG,             "GET_CONFIG" },             /* Get a counter configuration */
{ CtrDrvrSET_CONFIG,             "SET_CONFIG" },             /* Set a counter configuration */
{ CtrDrvrGET_PLL,                "GET_PLL" },                /* Get phase locked loop parameters */
{ CtrDrvrSET_PLL,                "SET_PLL" },                /* Set phase locked loop parameters */
{ CtrDrvrSET_PLL_ASYNC_PERIOD,   "SET_PLL_ASYNC_PERIOD" },   /* Set PLL asynchronous period */
{ CtrDrvrGET_PLL_ASYNC_PERIOD,   "GET_PLL_ASYNC_PERIOD" },   /* Get PLL asynchronous period */
{ CtrDrvrREAD_TELEGRAM,          "READ_TELEGRAM" },          /* Read telegrams from CTR */
{ CtrDrvrREAD_EVENT_HISTORY,     "READ_EVENT_HISTORY" },     /* Read incomming event history */
{ CtrDrvrHPTDC_OPEN,             "HPTDC_OPEN" },             /* Open HPTDC JTAG interface */
{ CtrDrvrHPTDC_IO,               "HPTDC_IO" },               /* Perform HPTDC IO operation */
{ CtrDrvrHPTDC_CLOSE,            "HPTDC_CLOSE" },            /* Close HPTDC JTAG interface */
{ CtrDrvrRAW_READ,               "RAW_READ" },               /* Raw read  access to mapped card for debug */
{ CtrDrvrRAW_WRITE,              "RAW_WRITE" },              /* Raw write access to mapped card for debug */
{ CtrDrvrGET_RECEPTION_ERRORS,   "GET_RECEPTION_ERRORS" },   /* Timing fram reception error status */
{ CtrDrvrGET_IO_STATUS,          "GET_IO_STATUS" },          /* Status of module inputs */
{ CtrDrvrGET_IDENTITY,           "GET_IDENTITY" },           /* Identity of board from ID chip */
{ CtrDrvrSET_DEBUG_HISTORY,      "SET_DEBUG_HISTORY" },      /* All events get logged in event history */
{ CtrDrvrSET_BRUTAL_PLL,         "SET_BRUTAL_PLL" },         /* Control how UTC PLL relocks */
{ CtrDrvrGET_MODULE_STATS,       "GET_MODULE_STATS" },       /* Get module statistics */
{ CtrDrvrSET_CABLE_ID,           "SET_CABLE_ID" },           /* Needed when no ID events sent */
{ CtrDrvrLOCK,                   "LOCK" },                   /* Lock all write access to the current CTR module configuration */
{ CtrDrvrUNLOCK,                 "UNLOCK" },                 /* Unlock all write access to the current CTR module configuration */
{ CtrDrvrSET_MODULE_BY_SLOT,     "SET_MODULE_BY_SLOT" },     /* Select the module to work with by slot ID */
{ CtrDrvrGET_MODULE_SLOT,        "GET_MODULE_SLOT" },        /* Get the slot ID of the selected module */
};

/* ==================== */

#define DEBUG_ARG_PRINT 4

void pbuf(char *cp, int iosz, uint32_t *uip)
{
	int i, isz;

	isz = iosz/sizeof(uint32_t);
	if (isz > DEBUG_ARG_PRINT)
		isz = DEBUG_ARG_PRINT;

	if (isz) {
		printk("%s:%s:Args:\n",ctr_major_name,cp);
		for (i=0; i<isz; i++) {
			if ((i % 4) == 0)
				printk("\n%02d:",i);
			printk("0x%08X-%d\t",uip[i],uip[i]);
		}
		printk("\n");
	}
}

/* ==================== */

struct ioctl_names_s *get_name(int ionr)
{
	int i;
	struct ioctl_names_s *entry;
	for (i=0; i<CtrDrvrLAST_IOCTL; i++) {
		entry = &ioctl_names[i];
		if (ionr == entry->number)
			return entry;
	}
	return NULL;
}

void debug_ioctl(int debl, int iodr, int iosz, unsigned int ionr, void *arb, int flg)
{
	struct ioctl_names_s *entry;

	if (!debl)
		return;

	if (flg) {
		printk("%s:", ctr_major_name);
		entry = get_name(ionr);
		if (entry) {
			printk("ioctl:%s:%d\n",entry->name,entry->number);
			if ((iodr & _IOC_WRITE) && (debl > 1))
				pbuf("from_user",iosz,arb);
		} else
			printk("ioctl:illegal:%d\n",ionr);

	} else
		if (iodr & _IOC_READ)
			pbuf("from_drvr",iosz,arb);
}

/* ========================================================== */
/* GetVersion, also used to check for Bus errors. See Ping    */
/* ========================================================== */

#ifndef COMPILE_TIME
#define COMPILE_TIME 0
#endif

static int GetVersion(CtrDrvrModuleContext *mcon,
		      CtrDrvrVersion       *ver)
{
	CtrDrvrMemoryMap *mmap;
	uint32_t          stat;    /* Module status */

	mmap = mcon->Map;
	ver->VhdlVersion  = ioread32be(&mmap->VhdlVersion);
	ver->DrvrVersion  = COMPILE_TIME;
	ver->HardwareType = CtrDrvrHardwareTypeNONE;

	stat  = ioread32be(&mmap->Status);
	stat &= CtrDrvrTypeStatusMask;

	if (stat != CtrDrvrTypeStatusMask) {
		if (stat & CtrDrvrStatusCTRI)
			ver->HardwareType = CtrDrvrHardwareTypeCTRI;

		if (stat & CtrDrvrStatusCTRP)
			ver->HardwareType = CtrDrvrHardwareTypeCTRP;

		if (stat & CtrDrvrStatusCTRV)
			ver->HardwareType = CtrDrvrHardwareTypeCTRV;
	}
	return OK;
}

/* ========================================================== */

static int PingModule(CtrDrvrModuleContext *mcon)
{
	CtrDrvrVersion ver;
	int cc;

	cc = GetVersion(mcon,&ver);
	if ((ver.HardwareType == CtrDrvrHardwareTypeCTRI)
	||  (ver.HardwareType == CtrDrvrHardwareTypeCTRP)
	||  (ver.HardwareType == CtrDrvrHardwareTypeCTRV))
		return OK;
	return SYSERR;
}

/* ========================================================== */
/* Enable Interrupt                                           */
/* ========================================================== */

static int EnableInterrupts(CtrDrvrModuleContext *mcon, int msk) {

	CtrDrvrMemoryMap *mmap; /* Module Memory map */
	uint16_t intcsr;        /* Plx control word  */
	struct pci_dev *dev;
	uint16_t *sp;
	mmap = mcon->Map;

	dev = mcon->dev;
	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);

	/* We want level triggered interrupts for Plx INT1  */
	/* for an active high. Level type interrupts do not */
	/* require clearing on the Plx chip it self.        */

	intcsr = Plx9030IntcsrLINT_ENABLE
	       | Plx9030IntcsrLINT_HIGH
	       | Plx9030IntcsrPCI_INT_ENABLE;

	sp = (uint16_t *) mcon->Local;
	iowrite16(intcsr, &(sp[PLX9030_INTCSR>>1])); /* Little Endian !! */

	mcon->InterruptEnable |= msk;
	iowrite32be(mcon->InterruptEnable,&mmap->InterruptEnable);

	return OK;
}
/* ========================================================== */
/* Disable Interrupt                                          */
/* ========================================================== */

static int DisableInterrupts(uint32_t                tndx,
			     uint32_t                midx,
			     CtrDrvrConnectionClass  clss,
			     CtrDrvrClientContext   *ccon)
{
	CtrDrvrModuleContext *mcon;
	CtrDrvrMemoryMap     *mmap;

	int i, cmsk, hmsk, cntr;

	cmsk = 1 << ccon->ClientIndex;
	mcon = &(Wa.ModuleContexts[midx]);

	if (clss != CtrDrvrConnectionClassHARD)
		cntr = mcon->Trigs[tndx].Counter;

	else if (tndx <= CtrDrvrCOUNTERS)
		cntr = tndx;

	else
		cntr = -1;

	if (cntr != -1) {
		if (mcon->HardClients[cntr] & ~cmsk)
			return OK;

		hmsk = 1 << cntr;
		for (i=0; i<CtrDrvrRamTableSIZE; i++) {
			if ((mcon->Trigs[i].Counter == cntr)
			&&  (mcon->Clients[i] & ~cmsk)) {
				return OK;
			}
		}

	} else
	       hmsk = 1 << tndx;

	if (mcon->HardClients[tndx] & ~cmsk)
		return OK;

	mmap = mcon->Map;
	mcon->InterruptEnable &= ~hmsk;
	iowrite32be(mcon->InterruptEnable,&mmap->InterruptEnable);

	return OK;
}

/* ========================================================== */
/* Reset a module                                             */
/* ========================================================== */

static void HptdcStateReset(uint32_t *jtg);
static uint32_t AutoShiftLeft(uint32_t mask, uint32_t value);

static void Reset(CtrDrvrModuleContext *mcon)
{

	CtrDrvrMemoryMap     *mmap;
	uint32_t             *jtg;
	int i;

	mmap = mcon->Map;

	iowrite32be(CtrDrvrCommandRESET,&mmap->Command);
	mcon->Status = CtrDrvrStatusNO_LOST_INTERRUPTS | CtrDrvrStatusNO_BUS_ERROR; /** Legacy remove this some time */

	msleep(10);

	mcon->Command &= ~CtrDrvrCommandSET_HPTDC; /* Hptdc needs to be re-initialized */
	iowrite32be(mcon->Command,&mmap->Command);

	for (i=CtrDrvrCounter1; i<=CtrDrvrCounter8; i++) {
		if (ioread32be(&mmap->Counters[i].Control.RemOutMask) == 0) {
			iowrite32be(AutoShiftLeft(CtrDrvrCntrCntrlOUT_MASK,(1 << i)) | CtrDrvrCntrCntrlTTL_BAR,
				    &mmap->Counters[i].Control.RemOutMask);
		}
	}

	if (mcon->Pll.KP == 0) {
		mcon->Pll.KP = 337326;
		mcon->Pll.KI = 901;
		mcon->Pll.NumAverage = 100;
		mcon->Pll.Phase = 1950;
	}
	Io32Write((uint32_t *) &(mmap->Pll),
		  (uint32_t *) &(mcon->Pll),
		  (uint32_t  ) sizeof(CtrDrvrPll));

	EnableInterrupts(mcon,mcon->InterruptEnable);

	jtg = &mmap->HptdcJtag;
	HptdcStateReset(jtg);           /* GOTO: Run test/idle */
}

/* ========================================================== */
/* Get Status                                                 */
/* ========================================================== */

static uint32_t GetStatus(CtrDrvrModuleContext *mcon)
{
	CtrDrvrMemoryMap *mmap;
	uint32_t          stat;

	mmap = mcon->Map;
	stat = ioread32be(&mmap->Status) & CtrDrvrHwStatusMASK;
	stat  = stat | (mcon->Status & ~CtrDrvrHwStatusMASK);
	return stat;
}

/* ========================================================== */
/* HPTDC JTAG Bit transfer                                    */
/* ========================================================== */

static uint32_t HptdcBitTransfer(uint32_t *jtg,
				 uint32_t  tdi,
				 uint32_t  tms)
{
	uint32_t jwd, tdo;

	jwd = HptdcJTAG_TRST | tdi | tms;
	iowrite32be(jwd,jtg);
	jwd = HptdcJTAG_TRST | HptdcJTAG_TCK | tdi | tms;
	iowrite32be(jwd,jtg);
	tdo = ioread32be(jtg);
	jwd = HptdcJTAG_TRST | tdi | tms;
	iowrite32be(jwd,jtg);
	if (tdo & HptdcJTAG_TDO)
		return 1;
	return 0;
}

/* ========================================================== */
/* HPTDC JTAG State machine reset                             */
/* ========================================================== */

static void HptdcStateReset(uint32_t *jtg)
{
	uint32_t jwd;

	jwd = HptdcJTAG_TRST; /* TRST is active on zero */
	iowrite32be(jwd,jtg);
	jwd = 0;              /* TCK and TRST are down */
	iowrite32be(jwd,jtg);
	jwd = HptdcJTAG_TCK;  /* Clock in reset on rising edge */
	iowrite32be(jwd,jtg);
	jwd = HptdcJTAG_TRST | HptdcJTAG_TCK;
	iowrite32be(jwd,jtg);
	jwd = HptdcJTAG_TRST; /* Remove TRST */
	iowrite32be(jwd,jtg);
	HptdcBitTransfer(jtg,0,0); /* GOTO: Run test/idle */
	return;
}

/* ========================================================== */
/* Execute an HPTDC command                                   */
/* ========================================================== */

static int HptdcCommand(CtrDrvrModuleContext *mcon,
			 HptdcCmd             cmd,
			 HptdcRegisterVlaue  *wreg,
			 HptdcRegisterVlaue  *rreg,    /* No read back if NULL */
			 int                  size,    /* Number of registers */
			 int                  pflg,    /* Parity flag */
			 int                  rflg) {  /* Reset state flag */

CtrDrvrMemoryMap *mmap;    /* Module Memory map */
uint32_t     *jtg;

uint32_t tdi, msk, parity, bits, wval, rval, cval;
int i, j;

   if (PingModule(mcon) == OK) {
      mmap = mcon->Map;
      jtg = &mmap->HptdcJtag;

      /* Cycle the state machine to get to selected command register */

      if (rflg) HptdcStateReset(jtg);           /* GOTO: Run test/idle */
      HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);    /* GOTO: Select DR scan */
      HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);    /* GOTO: Select IR scan */
      HptdcBitTransfer(jtg,0,0);                /* GOTO: Capture IR */
      HptdcBitTransfer(jtg,0,0);                /* GOTO: Shift IR */

      /* Select the IR by shifting in the register ID in cmd */

      parity = 0; cval = 0;
      for (i=0; i<HptdcCmdBITS; i++) {
	 msk = 1 << i;
	 if (msk & cmd) { tdi = HptdcJTAG_TDI; parity++; }
	 else           { tdi = 0;                       }
	 if (HptdcBitTransfer(jtg,tdi,0)) cval |= msk;
      }
      if (parity & 1) HptdcBitTransfer(jtg,HptdcJTAG_TDI,HptdcJTAG_TMS); /* GOTO: Exit 1 IR */
      else            HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);

      HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);    /* GOTO: Update IR */
      HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);    /* GOTO: Select DR scan */
      HptdcBitTransfer(jtg,0,0);                /* GOTO: Capture DR */
      HptdcBitTransfer(jtg,0,0);                /* GOTO: Shift DR */

      /* Write and read back register data */

      parity = 0;
      for (j=0; j<size; j++) {
	 bits = wreg[j].EndBit - wreg[j].StartBit + 1;
	 wval = wreg[j].Value;
	 rval = 0;
	 for (i=0; i<bits; i++) {
	    msk = 1 << i;
	    if (msk & wval) { tdi = HptdcJTAG_TDI; parity++; }
	    else            { tdi = 0;                       }

	    if (pflg) {
	       if (HptdcBitTransfer(jtg,tdi,0)) rval |= msk;
	    } else {
	       if (i==bits-1) {
		  if (HptdcBitTransfer(jtg,tdi,HptdcJTAG_TMS)) rval |= msk;
	       } else {
		  if (HptdcBitTransfer(jtg,tdi,0)) rval |= msk;
	       }
	    }
	 }
	 if (rreg) {
	    rreg[j].EndBit   = wreg[j].EndBit;
	    rreg[j].StartBit = wreg[j].StartBit;
	    rreg[j].Value    = rval;
	 }
      }
      if (pflg) {
	 if (parity & 1) HptdcBitTransfer(jtg,HptdcJTAG_TDI,HptdcJTAG_TMS); /* GOTO: Exit 1 DR */
	 else            HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);
      }

      HptdcBitTransfer(jtg,0,HptdcJTAG_TMS);    /* GOTO: Update DR */
      HptdcBitTransfer(jtg,0,0);                /* GOTO: Run test/idle */

      return OK;
   }
   return -ENXIO;
}

/* ========================================================== */
/* Read the time from the CTR                                 */
/* ========================================================== */

static CtrDrvrCTime *GetTime(CtrDrvrModuleContext *mcon) {

static CtrDrvrCTime t;

CtrDrvrMemoryMap *mmap;    /* Module Memory map */

   mmap = mcon->Map;
   iowrite32be(CtrDrvrCommandLATCH_UTC,&mmap->Command);

   t.CTrain          = ioread32be(&mmap->ReadTime.CTrain);
   t.Time.Second     = ioread32be(&mmap->ReadTime.Time.Second);
   t.Time.TicksHPTDC = ioread32be(&mmap->ReadTime.Time.TicksHPTDC);
   return &t;
}

/* ========================================================== */
/* Set the time on the CTR                                    */
/* ========================================================== */

static int SetTime(CtrDrvrModuleContext *mcon, uint32_t tod) {

CtrDrvrMemoryMap *mmap;    /* Module Memory map */

   if (PingModule(mcon) == OK) {
      mmap = mcon->Map;
      iowrite32be(tod,&mmap->SetTime);
      iowrite32be(CtrDrvrCommandSET_UTC,&mmap->Command);

      return OK;
   }
   return SYSERR;
}

/*=========================================================== */
/* Read event history                                         */
/*=========================================================== */

static void ReadEventHistory(CtrDrvrModuleContext *mcon, CtrDrvrEventHistoryBuf *evhs)
{
	CtrDrvrMemoryMap *mmap = mcon->Map;
	CtrDrvrEventHistoryEntry *dst, *src;
	uint32_t indx;
	int i, j;

		indx = ioread32be(&mmap->EventHistory.Index);
		if (indx >= CtrDrvrHISTORY_TABLE_SIZE)
			indx = 0;

		for (i=0,j=indx; i<CtrDrvrHISTORY_BUF_SIZE; i++,j--) {
			dst = &(evhs->Entries[i]);

			if (j<0)
				j = CtrDrvrHISTORY_TABLE_SIZE -1;
			src = &(mmap->EventHistory.Entries[j]);

			Io32Read((uint32_t *) dst,
				 (uint32_t *) src,
				 (uint32_t  ) sizeof(CtrDrvrEventHistoryEntry));
		}
		evhs->Index = 0;
}

/*=========================================================== */
/* Raw IO                                                     */
/* The correct PLX9030 endian settings must have been made    */
/*=========================================================== */

static void RawIo(CtrDrvrModuleContext *mcon,
		 uint32_t              *ibuf,
		 uint32_t               icnt,
		 uint32_t               ioff,
		 uint32_t               flag)
{
	uint32_t *mmap = (uint32_t *) mcon->Map;
	int       i, j;

	for (i=0,j=ioff; i<icnt; i++,j++) {
		if (flag)
			iowrite32be(ibuf[i],&mmap[j]);
		else
			ibuf[i] = ioread32be(&mmap[j]);
	}
}

/* ========================================================== */
/* Auto shift left  a value for a given mask                  */
/* ========================================================== */

static uint32_t AutoShiftLeft(uint32_t mask, uint32_t value) {
int i,m;

   m = mask;
   for (i=0; i<32; i++) {
      if (m & 1) break;
      m >>= 1;
   }
   return (value << i) & mask;
}

/* ========================================================== */
/* Auto shift right a value for a given mask                  */
/* ========================================================== */

static uint32_t AutoShiftRight(uint32_t mask, uint32_t value) {
int i,m;

   m = mask;
   for (i=0; i<32; i++) {
      if (m & 1) break;
      m >>= 1;
   }
   return (value >> i) & m;
}

/* ========================================================== */
/* Convert driver API trigger to compact hardware form.       */
/* The API form for a trigger is too big to fit in the FPGA.  */
/* We keep a propper structure for the driver API, and        */
/* declare a second version packed into bit fields for the    */
/* hardware representation.                                   */
/* ========================================================== */

static CtrDrvrHwTrigger *TriggerToHard(CtrDrvrTrigger *strg) {
static CtrDrvrHwTrigger htrg;
uint32_t trigger;

   trigger  = AutoShiftLeft(CtrDrvrTrigGROUP_VALUE_MASK ,strg->Group.GroupValue);
   trigger |= AutoShiftLeft(CtrDrvrTrigCONDITION_MASK   ,strg->TriggerCondition);
   trigger |= AutoShiftLeft(CtrDrvrTrigMACHINE_MASK     ,strg->Machine);
   trigger |= AutoShiftLeft(CtrDrvrTrigCOUNTER_MASK     ,strg->Counter);
   trigger |= AutoShiftLeft(CtrDrvrTrigGROUP_NUMBER_MASK,strg->Group.GroupNumber);

   htrg.Frame   = strg->Frame;
   htrg.Trigger = trigger;

   return &htrg;
}

/* ========================================================== */
/* Convert compact hardware trigger to driver API trigger     */
/* ========================================================== */

static CtrDrvrTrigger *HardToTrigger(CtrDrvrHwTrigger *htrg) {
static CtrDrvrTrigger strg;
uint32_t trigger;

   strg.Frame.Long = ioread32be(&htrg->Frame);
   trigger         = ioread32be(&htrg->Trigger);

   strg.Group.GroupValue  = AutoShiftRight(CtrDrvrTrigGROUP_VALUE_MASK ,trigger);
   strg.TriggerCondition  = AutoShiftRight(CtrDrvrTrigCONDITION_MASK   ,trigger);
   strg.Machine           = AutoShiftRight(CtrDrvrTrigMACHINE_MASK     ,trigger);
   strg.Counter           = AutoShiftRight(CtrDrvrTrigCOUNTER_MASK     ,trigger);
   strg.Group.GroupNumber = AutoShiftRight(CtrDrvrTrigGROUP_NUMBER_MASK,trigger);

   return &strg;
}

/* ================================================================================== */
/* Convert a soft counter configuration to compact hardware form.                     */
/* ================================================================================== */

static CtrDrvrHwCounterConfiguration *ConfigToHard(CtrDrvrCounterConfiguration *scnf) {
static CtrDrvrHwCounterConfiguration hcnf;
uint32_t config;

   config  = AutoShiftLeft(CtrDrvrCounterConfigPULSE_WIDTH_MASK,scnf->PulsWidth);
   config |= AutoShiftLeft(CtrDrvrCounterConfigCLOCK_MASK      ,scnf->Clock);
   config |= AutoShiftLeft(CtrDrvrCounterConfigMODE_MASK       ,scnf->Mode);
   config |= AutoShiftLeft(CtrDrvrCounterConfigSTART_MASK      ,scnf->Start);
   config |= AutoShiftLeft(CtrDrvrCounterConfigON_ZERO_MASK    ,scnf->OnZero);

   hcnf.Config = config;
   hcnf.Delay  = scnf->Delay;

   return &hcnf;
}

/* ================================================================================== */
/* Convert a compacr hardware counter configuration to API form.                      */
/* ================================================================================== */

static CtrDrvrCounterConfiguration *HardToConfig(CtrDrvrHwCounterConfiguration *hcnf) {
static CtrDrvrCounterConfiguration scnf;
uint32_t config;

   config     = ioread32be(&hcnf->Config);
   scnf.Delay = ioread32be(&hcnf->Delay);

   scnf.OnZero    = AutoShiftRight(CtrDrvrCounterConfigON_ZERO_MASK    ,config);
   scnf.Start     = AutoShiftRight(CtrDrvrCounterConfigSTART_MASK      ,config);
   scnf.Mode      = AutoShiftRight(CtrDrvrCounterConfigMODE_MASK       ,config);
   scnf.Clock     = AutoShiftRight(CtrDrvrCounterConfigCLOCK_MASK      ,config);
   scnf.PulsWidth = AutoShiftRight(CtrDrvrCounterConfigPULSE_WIDTH_MASK,config);

   return &scnf;
}

/* ========================================================== */
/* Get a PTIM module number                                   */
/* ========================================================== */

static uint32_t GetPtimModule(uint32_t eqpnum) {
int i;

   for (i=0; i<Wa.Ptim.Size; i++) {
      if (eqpnum == Wa.Ptim.Objects[i].EqpNum) {
	 return Wa.Ptim.Objects[i].ModuleIndex + 1;
      }
   }
   return 0;
}

/* ========================================================== */
/* Connect to a timing object                                 */
/* ========================================================== */

static int Connect(CtrDrvrConnection    *conx,
		   CtrDrvrClientContext *ccon)
{
	CtrDrvrModuleContext       *mcon;
	CtrDrvrMemoryMap           *mmap;
	CtrDrvrTrigger              trig;
	CtrDrvrCounterConfiguration conf;
	CtrDrvrEventFrame           frme;

	int i, midx, cmsk, hmsk, count, valu;

	/* Get the module to make the connection on */

	midx = conx->Module -1;
	if ((midx < 0) || (midx >= CtrDrvrMODULE_CONTEXTS))
		midx = ccon->ModuleIndex;

	/* Set up connection mask and module map and context */

	cmsk  = 1 << ccon->ClientIndex;
	mcon  = &(Wa.ModuleContexts[midx]);
	mmap  =  mcon->Map;

	/* Connect to an existing PTIM instance */

	count = 0;   /* Number of connections made so far is zero */

	if (conx->EqpClass == CtrDrvrConnectionClassPTIM) {
		midx = GetPtimModule(conx->EqpNum) -1;
		if (midx >= 0) {
			mcon = &(Wa.ModuleContexts[midx]);
			mmap = mcon->Map;
			for (i=0; i<CtrDrvrRamTableSIZE; i++) {
				if ((conx->EqpNum   == mcon->EqpNum[i])
				&&  (conx->EqpClass == mcon->EqpClass[i])) {

					count++;

					mcon->Clients[i]      |= cmsk;
					mcon->InterruptEnable |= (1 << mcon->Trigs[i].Counter);

					valu = ioread32be(&mmap->Configs[i].Config);
					valu |= AutoShiftLeft(CtrDrvrCounterConfigON_ZERO_MASK, CtrDrvrCounterOnZeroBUS);

					iowrite32be(valu,&mmap->Configs[i].Config);
				}
			}
		}

	} else if (conx->EqpClass == CtrDrvrConnectionClassCTIM) {

		/* Check to see if there is already an instance, and connect to it */

		for (i=0; i<CtrDrvrRamTableSIZE; i++) {
			if ((conx->EqpNum   == mcon->EqpNum[i])
			&&  (conx->EqpClass == mcon->EqpClass[i])) {

				mcon->Clients[i]        |= cmsk;
				mcon->InterruptEnable   |= CtrDrvrInterruptMaskCOUNTER_0;

				valu = ioread32be(&mmap->Configs[i].Config);
				valu |= AutoShiftLeft(CtrDrvrCounterConfigON_ZERO_MASK, CtrDrvrCounterOnZeroBUS);

				iowrite32be(valu,&mmap->Configs[i].Config);
				count = 1;
				break;
			}
		}

		/* Need to create a new CTIM instance, so first find the CTIM object */

		if (count == 0) {
			frme.Struct = (CtrDrvrFrameStruct) {0,0,0};    /* No frame has been found yet */
			for (i=0; i<Wa.Ctim.Size; i++) {
				if (conx->EqpNum == Wa.Ctim.Objects[i].EqpNum) {
					frme = Wa.Ctim.Objects[i].Frame;
					break;
				}
			}

			/* If we found the CTIM object, look for an empty slot and create the instance */

			if (frme.Struct.Header != 0) {
				for (i=0; i<CtrDrvrRamTableSIZE; i++) {
					if (mcon->EqpNum[i] == 0) {      /* Is the slot empty ? */
						mcon->EqpNum[i]   = conx->EqpNum;
						mcon->EqpClass[i] = CtrDrvrConnectionClassCTIM;

						trig.Ctim             = conx->EqpNum;
						trig.Frame            = frme;
						trig.TriggerCondition = CtrDrvrTriggerConditionNO_CHECK;
						trig.Machine          = CtrDrvrMachineNONE;
						trig.Counter          = CtrDrvrCounter0;
						trig.Group            = (CtrDrvrTgmGroup) {0,0}; /* Number and Value */
						mcon->Trigs[i]        = trig;

						conf.OnZero           = CtrDrvrCounterOnZeroBUS;
						conf.Start            = CtrDrvrCounterStartNORMAL;
						conf.Mode             = CtrDrvrCounterModeNORMAL;
						conf.Clock            = CtrDrvrCounterClock1KHZ;
						conf.Delay            = 0;
						conf.PulsWidth        = 0;
						mcon->Configs[i]      = conf;

						Io32Write((uint32_t *) &(mmap->Trigs[i]),
							  (uint32_t *) TriggerToHard(&trig),
							  (uint32_t  ) sizeof(CtrDrvrHwTrigger));

						Io32Write((uint32_t *) &(mmap->Configs[i]),
							  (uint32_t *) ConfigToHard(&conf),
							  (uint32_t  ) sizeof(CtrDrvrHwCounterConfiguration));

						mcon->Clients[i]      |= cmsk;
						mcon->InterruptEnable |= CtrDrvrInterruptMaskCOUNTER_0;
						count = 1;
						break;
					}
				}

				/* Give a NOMEM error if no empty slot available */

				if (count == 0)
					return -ENOMEM;
			}
		}

	} else {
		for (i=0; i<CtrDrvrInterruptSOURCES; i++) {
			hmsk = 1 << i;
			if (conx->EqpNum & hmsk) {
				count++;
				mcon->InterruptEnable |= hmsk;
				mcon->HardClients[i]  |= cmsk;
			 }
		}
	}

	if (count)
		return EnableInterrupts(mcon,mcon->InterruptEnable);

	/* If count is zero, no connection was made, so give an error */

	return -ENXIO;
}

/* ========================================================== */
/* Disconnect from one specified trigger                      */
/* ========================================================== */

static int DisConnectOne(uint32_t           tndx,
			 uint32_t           midx,
			 CtrDrvrConnectionClass clss,
			 CtrDrvrClientContext  *ccon) {

CtrDrvrModuleContext       *mcon;
CtrDrvrMemoryMap           *mmap;
CtrDrvrTrigger              trig;
CtrDrvrCounterConfiguration conf;
uint32_t ncmsk, cmsk;
int valu;

   cmsk = 1 << ccon->ClientIndex; ncmsk = ~cmsk;
   mcon = &(Wa.ModuleContexts[midx]);
   mmap = mcon->Map;

   if ((clss == CtrDrvrConnectionClassCTIM) || (clss == CtrDrvrConnectionClassPTIM)) {
      if (cmsk & mcon->Clients[tndx]) {
	 mcon->Clients[tndx] &= ncmsk;
	 if (mcon->Clients[tndx] == 0) {
	    DisableInterrupts(tndx,midx,clss,ccon);
	    if (mcon->EqpClass[tndx] == CtrDrvrConnectionClassCTIM) {

	       memset(&trig,0,sizeof(CtrDrvrTrigger));
	       memset(&conf,0,sizeof(CtrDrvrCounterConfiguration));

	       Io32Write((uint32_t *) &(mmap->Trigs[tndx]),
			 (uint32_t *) TriggerToHard(&trig),
			 (uint32_t  ) sizeof(CtrDrvrHwTrigger));

	       Io32Write((uint32_t *) &(mmap->Configs[tndx]),
			 (uint32_t *) ConfigToHard(&conf),
			 (uint32_t  ) sizeof(CtrDrvrHwCounterConfiguration));

	       mcon->EqpNum[tndx]   = 0;
	       mcon->EqpClass[tndx] = 0;

	    } else {

	       valu = ioread32be(&mmap->Configs[tndx].Config);
	       valu &= ~(AutoShiftLeft(CtrDrvrCounterConfigON_ZERO_MASK, CtrDrvrCounterOnZeroBUS));
	       iowrite32be(valu,&mmap->Configs[tndx].Config);
	    }
	 }
      }
   }

   if (clss == CtrDrvrConnectionClassHARD) {
      if (mcon->HardClients[tndx]) {
	 mcon->HardClients[tndx] &= ncmsk;
	 if (mcon->HardClients[tndx] == 0) {
	    DisableInterrupts(tndx,midx,clss,ccon);
	 }
      }
   }

   return OK;
}

/* ========================================================== */
/* Disconnect from a timing object                            */
/* ========================================================== */

static int DisConnect(CtrDrvrConnection    *conx,
		      CtrDrvrClientContext *ccon) {

CtrDrvrModuleContext *mcon;
int i, midx;

   midx = conx->Module -1;
   mcon = &(Wa.ModuleContexts[midx]);

   if (conx->EqpClass != CtrDrvrConnectionClassHARD) {
      for (i=0; i<CtrDrvrRamTableSIZE; i++)
	 if ((conx->EqpNum   == mcon->EqpNum[i])
	 &&  (conx->EqpClass == mcon->EqpClass[i])) DisConnectOne(i,midx,conx->EqpClass,ccon);

   } else for (i=0; i<CtrDrvrInterruptSOURCES; i++) DisConnectOne(i,midx,CtrDrvrConnectionClassHARD,ccon);

   return OK;
}

/* ========================================================== */
/* Disconnect from all timing objects                         */
/* ========================================================== */

static int DisConnectAll(CtrDrvrClientContext *ccon) {

CtrDrvrModuleContext *mcon;
int i, midx;

   for (midx=0; midx<Wa.Modules; midx++) {
      mcon = &(Wa.ModuleContexts[midx]);
      for (i=0; i<CtrDrvrRamTableSIZE;     i++) {
	 if (mcon->EqpNum[i]) {
	    DisConnectOne(i,midx,mcon->EqpClass[i],ccon);
	 }
      }
      for (i=0; i<CtrDrvrInterruptSOURCES; i++) DisConnectOne(i,midx,CtrDrvrConnectionClassHARD,ccon);
   }
   return OK;
}

/* ========================================================== */
/* The ISR                                                    */
/* ========================================================== */

static irqreturn_t ctr_irq(int irq, void *arg) {

	CtrDrvrModuleContext *mcon = arg;

	uint32_t isrc, clients, msk, i, hlock, tndx = 0;
	unsigned char inum;

	unsigned long flags;

	CtrDrvrMemoryMap      *mmap = NULL;
	CtrDrvrFpgaCounter    *fpgc = NULL;
	CtrDrvrCounterHistory *hist = NULL;

	CtrDrvrQueue          *queue;
	CtrDrvrClientContext  *ccon;
	CtrDrvrReadBuf         rbf;

	mmap = mcon->Map;
	isrc = ioread32be(&mmap->InterruptSource);
	if (!isrc)
		return IRQ_NONE;

	for (inum=0; inum<CtrDrvrInterruptSOURCES; inum++) {
		msk = 1 << inum;
		if (msk & isrc) {
			memset(&rbf,0,sizeof(CtrDrvrReadBuf));
			clients = 0;                            /* No clients yet */

			if (inum < CtrDrvrCOUNTERS) {           /* Counter Interrupt ? */
				fpgc = &(mmap->Counters[inum]); /* Get counter FPGA configuration */
				hist = &(fpgc->History);        /* History of counter */

				if (ioread32be(&fpgc->Control.LockConfig) == 0) {    /* If counter not in remote */
					tndx = ioread32be(&hist->Index);             /* Index into trigger table */
					if (tndx < CtrDrvrRamTableSIZE) {
						clients = mcon->Clients[tndx];       /* Clients connected to timing objects */
						if (clients) {
							rbf.TriggerNumber = tndx +1;                  /* Trigger that loaded counter */
							rbf.Frame.Long    = ioread32be(&hist->Frame); /* Actual event that interrupted */

							rbf.TriggerTime.Time.Second     = ioread32be(&hist->TriggerTime.Time.Second);
							rbf.TriggerTime.Time.TicksHPTDC = ioread32be(&hist->TriggerTime.Time.TicksHPTDC);
							rbf.TriggerTime.CTrain          = ioread32be(&hist->TriggerTime.CTrain);

							rbf.StartTime.Time.Second       = ioread32be(&hist->StartTime.Time.Second);
							rbf.StartTime.Time.TicksHPTDC   = ioread32be(&hist->StartTime.Time.TicksHPTDC);
							rbf.StartTime.CTrain            = ioread32be(&hist->StartTime.CTrain);

							rbf.OnZeroTime.Time.Second      = ioread32be(&hist->OnZeroTime.Time.Second);
							rbf.OnZeroTime.Time.TicksHPTDC  = ioread32be(&hist->OnZeroTime.Time.TicksHPTDC);
							rbf.OnZeroTime.CTrain           = ioread32be(&hist->OnZeroTime.CTrain);

							hlock = ioread32be(&fpgc->Control.LockHistory);
							if (hlock > 1)
								mcon->Status &= ~CtrDrvrStatusNO_LOST_INTERRUPTS;

							/* Copy the rest of the information from the module context */

							rbf.Ctim                = mcon->Trigs[tndx].Ctim;
							rbf.Connection.Module   = mcon->ModuleIndex +1;
							rbf.Connection.EqpClass = mcon->EqpClass[tndx];
							rbf.Connection.EqpNum   = mcon->EqpNum[tndx];
						}
					}
				}
			}

		       if (clients == 0) {
				clients = mcon->HardClients[inum];   /* Hardware only connection */
				if (clients) {
					if (inum < CtrDrvrCOUNTERS) {
						rbf.OnZeroTime.Time.Second      = ioread32be(&hist->OnZeroTime.Time.Second);
						rbf.OnZeroTime.Time.TicksHPTDC  = ioread32be(&hist->OnZeroTime.Time.TicksHPTDC);
						rbf.OnZeroTime.CTrain           = ioread32be(&hist->OnZeroTime.CTrain);
					} else
						rbf.OnZeroTime = *GetTime(mcon);

					rbf.Connection.Module   = mcon->ModuleIndex +1;
					rbf.Connection.EqpClass = CtrDrvrConnectionClassHARD;
					rbf.Connection.EqpNum   = msk;
				}
		       }

		       /* If client is connected to a hardware interrupt, and   */
		       /* other clients are connected to a timing object on the */
		       /* same counter; the client needs the interrupt number   */
		       /* to know from where the interrupt came. */

		       rbf.InterruptNumber = inum; /* Source of interrupt, Counter or other */

		       /* Place read buffer on connected clients queues */

		       clients |= mcon->HardClients[inum];
		       for (i=0; i<CtrDrvrCLIENT_CONTEXTS; i++) {
				if (clients & (1 << i)) {
					ccon = &(Wa.ClientContexts[i]);
					queue = &(ccon->Queue);

					spin_lock_irqsave(&ccon->Lock,flags);

					queue->Entries[queue->WrPntr] = rbf;
					queue->WrPntr = (queue->WrPntr + 1) % CtrDrvrQUEUE_SIZE;

					if (queue->Size < CtrDrvrQUEUE_SIZE)

						queue->Size++;

					else {

						queue->Missed++;
						queue->RdPntr = (queue->RdPntr + 1) % CtrDrvrQUEUE_SIZE;
					}

					spin_unlock_irqrestore(&ccon->Lock,flags);
					wake_up(&ccon->Wq);
				}
			}
		}
	}
	return IRQ_HANDLED;
}

/*========================================================================*/
/* OPEN                                                                   */
/*========================================================================*/

static int __ctr_open(struct inode *inode, struct file *filp)
{
	CtrDrvrClientContext *ccon;
	uint32_t cindx;

	for (cindx=0; cindx<CtrDrvrCLIENT_CONTEXTS; cindx++) {
		ccon = &(Wa.ClientContexts[cindx]);
		if (ccon->InUse)
			continue;
		break;
	}

	if (cindx >= CtrDrvrCLIENT_CONTEXTS)
		return -EBUSY;

	memset(ccon,0,sizeof(CtrDrvrClientContext));

	ccon->ClientIndex = cindx;
	ccon->InUse       = 1;
	ccon->Pid         = current->pid;
	ccon->Timeout     = CtrDrvrDEFAULT_TIMEOUT;

	spin_lock_init(&ccon->Lock);
	init_waitqueue_head(&ccon->Wq);

	filp->private_data = ccon;

	return 0;
}

int ctr_open(struct inode *inode, struct file *filp)
{
	int cc;
	mutex_lock(&ctr_drvr_mutex);
	cc = __ctr_open(inode,filp);
	mutex_unlock(&ctr_drvr_mutex);
	return cc;
}

/*========================================================================*/
/* CLOSE                                                                  */
/*========================================================================*/

int __ctr_close(struct inode *inode, struct file *filp)
{
	CtrDrvrClientContext *ccon = (CtrDrvrClientContext *) filp->private_data;

	DisConnectAll(ccon);
	ccon->InUse = 0;

	filp->private_data = NULL;
	return 0;
}

int ctr_close(struct inode *inode, struct file *filp)
{
	int cc;
	mutex_lock(&ctr_drvr_mutex);
	cc = __ctr_close(inode,filp);
	mutex_unlock(&ctr_drvr_mutex);
	return cc;
}

/*========================================================================*/
/* READ                                                                   */
/*========================================================================*/

ssize_t ctr_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	CtrDrvrClientContext *ccon = (CtrDrvrClientContext *) filp->private_data;

	CtrDrvrQueue   *queue;
	size_t          wcnt;
	unsigned long   flags;
	int             cc;

	if (ccon->DebugOn)
		printk("ctr_read: PID:%d\n",ccon->Pid);

	queue = &ccon->Queue;
	if (queue->QueueOff) {
		spin_lock_irqsave(&ccon->Lock,flags);
		queue->Size   = 0;
		queue->Missed = 0;
		queue->WrPntr = 0;
		queue->RdPntr = 0;
		spin_unlock_irqrestore(&ccon->Lock,flags);
	}

	if (!queue->Size) {
		if (ccon->Timeout)
			cc = wait_event_interruptible_timeout(ccon->Wq,
							      queue->Size > 0,
							      ccon->Timeout);
		else
			cc = wait_event_interruptible(ccon->Wq,
						      queue->Size > 0);
		if (cc <= 0) {
			if (cc == 0)
				cc = -ETIMEDOUT;
			return cc;
		}
	}

	wcnt = count;
	if (wcnt > sizeof(CtrDrvrReadBuf))
		wcnt = sizeof(CtrDrvrReadBuf);

	spin_lock_irqsave(&ccon->Lock,flags);
	cc = copy_to_user(buf, &queue->Entries[queue->RdPntr], wcnt);
	queue->RdPntr = (queue->RdPntr + 1) % CtrDrvrQUEUE_SIZE;
	queue->Size--;
	spin_unlock_irqrestore(&ccon->Lock,flags);

	if (cc)
		return -EACCES;

	if (ccon->DebugOn)
		printk("ctr_read:PID:%d OK\n",ccon->Pid);

	return wcnt;
}

/*========================================================================*/
/* WRITE                                                                  */
/* Write is a way of simulating incomming events, calling write with the  */
/* appropriate values in the buffer will cause waiting clients to wake up */
/* with the supplied CtrDrvrReadBuf.                                     */
/*========================================================================*/

ssize_t ctr_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	CtrDrvrClientContext *ccon = (CtrDrvrClientContext *) filp->private_data;

	CtrDrvrModuleContext *mcon;
	CtrDrvrQueue         *queue;
	CtrDrvrConnection    *conx;
	CtrDrvrReadBuf        rb;
	CtrDrvrWriteBuf       wb;
	unsigned long         flags;
	uint32_t              clients = 0;
	int                   cc, i, tndx, midx, hmsk;

	size_t          wcnt;

	wcnt = count;
	if (wcnt > sizeof(CtrDrvrWriteBuf))
		wcnt = sizeof(CtrDrvrWriteBuf);

	cc = copy_from_user(&wb,buf,wcnt);
	if (cc)
		return -EACCES;

	conx = &(wb.Connection);

	midx = conx->Module -1;
	if ((midx<0) || (midx>=CtrDrvrMODULE_CONTEXTS))
		return -EACCES;

	mcon = &(Wa.ModuleContexts[midx]);

	memset(&rb,0,sizeof(CtrDrvrReadBuf));

	if (conx->EqpClass != CtrDrvrConnectionClassHARD) {
		if (wb.TriggerNumber == 0) {

			/* This code provokes the first matching trigger */

			for (tndx=0; tndx<CtrDrvrRamTableSIZE; tndx++) {
				if ((mcon->EqpNum[tndx]   == conx->EqpNum)
				&&  (mcon->EqpClass[tndx] == conx->EqpClass)) {
					clients = mcon->Clients[tndx];
					rb.TriggerNumber = tndx +1;
					rb.Frame = mcon->Trigs[tndx].Frame;
					rb.Frame.Struct.Value = wb.Payload;
					rb.Ctim = mcon->Trigs[tndx].Ctim;
					rb.InterruptNumber = mcon->Trigs[tndx].Counter;
					break;
				}
			}
		} else {
			tndx = wb.TriggerNumber -1;
			clients = mcon->Clients[tndx];
			rb.TriggerNumber = tndx +1;
			rb.Frame = mcon->Trigs[tndx].Frame;
			rb.Ctim = mcon->Trigs[tndx].Ctim;
			rb.InterruptNumber = mcon->Trigs[tndx].Counter;
		}
	} else {
		for (i=0; i<CtrDrvrInterruptSOURCES; i++) {
			hmsk = 1 << i;
			if (conx->EqpNum & hmsk) {
				clients |= mcon->HardClients[i];
				rb.InterruptNumber = i + 1;
			}
		}
	}

	/* Place read buffer on connected clients queues */

	if (clients) {

		rb.Connection = *conx;
		rb.TriggerTime = *GetTime(mcon);
		rb.StartTime = rb.TriggerTime;
		rb.OnZeroTime = rb.TriggerTime;

		for (i=0; i<CtrDrvrCLIENT_CONTEXTS; i++) {
			if (clients & (1 << i)) {
				ccon = &(Wa.ClientContexts[i]);
				queue = &(ccon->Queue);

				spin_lock_irqsave(&ccon->Lock,flags);
				queue->Entries[queue->WrPntr] = rb;
				queue->WrPntr = (queue->WrPntr + 1) % CtrDrvrQUEUE_SIZE;
				if (queue->Size < CtrDrvrQUEUE_SIZE) {
					queue->Size++;
					wake_up(&ccon->Wq);
				} else {
					queue->Missed++;
					queue->RdPntr = (queue->RdPntr + 1) % CtrDrvrQUEUE_SIZE;
				}
				spin_unlock_irqrestore(&ccon->Lock,flags);
			}
		}
	}
	return sizeof(CtrDrvrConnection);
}

/*========================================================================*/
/* Install driver and initialize hardware                                 */
/*========================================================================*/

#define CTRP_BARS 0x5

struct file_operations ctr_fops;

int ctr_install(void)
{
	CtrDrvrModuleContext *mcon;

	uint32_t *vadr;
	int modix, cc, j;

	CtrDrvrMemoryMap *mmap;
	mod_par_t        *mpar;

	struct pci_dev *pcur;

	if (check_args(ctr_major_name) == 0)
		printk("ctr_install: check_args failed, defaulting installation\n");

	if (!init_mod_pars(ctr_major_name,CERN_VENDOR_ID,CTRP_DEVICE_ID,CTRP_BARS)) {
		printk("ctr_install: No hardware installed\n");
		return -ENODEV;
	}

	cc = register_chrdev(ctr_major, ctr_major_name, &ctr_fops);
	if (cc < 0)
		return cc;
	ctr_major = cc;

	for (modix=0; modix<CtrDrvrMODULE_CONTEXTS; modix++) {
		mpar = &mods[modix];
		if (!mpar->dev) break;

		mcon = &(Wa.ModuleContexts[modix]);
		mcon->PciSlot     = mpar->pci_slot_num;
		mcon->ModuleIndex = modix;
		mcon->dev         = mpar->dev;
		mcon->InUse       = 1;

		vadr = (uint32_t *) mpar->map[2];
		mcon->Map = (CtrDrvrMemoryMap *) vadr;

		vadr = (uint32_t *) mpar->map[0];
		mcon->Local = (uint32_t *) vadr;

		pcur = mcon->dev;
		cc = request_irq(pcur->irq, ctr_irq, IRQF_SHARED, ctr_major_name, mcon);
		if (cc < 0) {
		       pci_disable_device(mcon->dev);
		       printk("ctr_install:request_irq:ERROR%d\n",cc);
		       return cc;
		}

		/* Wipe out any old triggers left in Ram after a warm reboot */

		mmap = mcon->Map;
		for (j=0; j<CtrDrvrRamTableSIZE; j++) {
			iowrite32be(0,&mmap->Trigs[j].Frame.Long);
			iowrite32be(0,&mmap->Trigs[j].Trigger);
		}

		Reset(mcon);
	}

	Wa.Modules = iluns;
	return 0;
}

/*========================================================================*/
/* Uninstall                                                              */
/*========================================================================*/

static void release_device(struct pci_dev *pdev, void *mem, int bar)
{
	pci_iounmap(pdev, (void __iomem *) mem);
	pci_release_region(pdev, bar);
	pci_disable_device(pdev);
	pci_dev_put(pdev);
}

void ctr_uninstall(void)
{
	CtrDrvrMemoryMap *mmap;
	int i;
	uint32_t src;

	CtrDrvrModuleContext *mcon;

	for (i=0; i<CtrDrvrMODULE_CONTEXTS; i++) {
		mcon = &(Wa.ModuleContexts[i]);
		if (mcon->InUse) {

			mmap = mcon->Map;
			src = mmap->InterruptSource;
			iowrite32be(0,&mmap->InterruptEnable);
			free_irq(mcon->dev->irq, mcon);

			release_device(mcon->dev, mcon->Local, 0);
			release_device(mcon->dev, mcon->Map, 2);
		}
	}
	unregister_chrdev(ctr_major,ctr_major_name);
	printk("%s:Driver uninstalled\n",ctr_major_name);
}

/*========================================================================*/
/* IOCTL                                                                  */
/*========================================================================*/

long __ctr_ioctl(struct file *filp, uint32_t cmd, unsigned long arg)
{
	CtrDrvrClientContext *ccon = (CtrDrvrClientContext *) filp->private_data;

	CtrDrvrModuleContext           *mcon;
	CtrDrvrConnection              *conx;
	CtrDrvrClientList              *cls;
	CtrDrvrClientConnections       *ccn;
	CtrDrvrRawIoBlock              *riob;
	CtrDrvrVersion                 *ver;
	CtrDrvrModuleAddress           *moad;
	CtrDrvrCTime                   *ctod;
	CtrDrvrHwTrigger               *htrg;
	CtrDrvrTrigger                 *strg;
	CtrDrvrAction                  *act;
	CtrDrvrCtimBinding             *ctim;
	CtrDrvrPtimBinding             *ptim;
	CtrDrvrCtimObjects             *ctimo;
	CtrDrvrPtimObjects             *ptimo;
	CtrDrvrHptdcIoBuf              *hpio;
	CtrDrvrCounterConfigurationBuf *conf;
	CtrDrvrCounterHistoryBuf       *hisb;
	CtrdrvrRemoteCommandBuf        *remc;
	CtrDrvrCounterMaskBuf          *cmsb;
	CtrDrvrPll                     *pll;
	CtrDrvrPllAsyncPeriodNs        *asyp;
	CtrDrvrTgmBuf                  *tgmb;
	CtrDrvrEventHistoryBuf         *evhs;
	CtrDrvrReceptionErrors         *rcpe;
	CtrDrvrBoardId                 *bird;
	CtrDrvrModuleStats             *mods;

	CtrDrvrMemoryMap   *mmap;

	int cc, i, j, k, n, found, size, start;
	uint32_t msk, hmsk, *iobuf;

	uint32_t lav, *lap;
	uint16_t sav;

	int iodr, iosz, ionr;
	void *arb;

	iodr = _IOC_DIR(cmd);
	iosz = _IOC_SIZE(cmd);
	ionr = _IOC_NR(cmd);

	if ((arb = kmalloc(iosz, GFP_KERNEL)) == NULL)
		return -ENOMEM;

	/**
	 * In some legacy code, the arg can be NULL even though all IOCTL calls pass at least a uint32_t !!
	 * N.B. kmalloc has initialized memory allocated to zero.
	 */

	if ((arg) && (iodr & _IOC_WRITE) && copy_from_user(arb, (void *) arg, iosz)) {
		cc = -EACCES;
		goto out;
	}

	lav = *((uint32_t *) arb);
	lap =   (uint32_t *) arb ;
	sav = lav;

	mcon = &(Wa.ModuleContexts[ccon->ModuleIndex]);
	mmap = (CtrDrvrMemoryMap *) mcon->Map;

	debug_ioctl(ccon->DebugOn, iodr, iosz, ionr, arb, 1);

	cc = 0;
	switch (cmd) {

		case CtrIoctlSET_SW_DEBUG:
			if (lav) ccon->DebugOn = !!lav;
			else     ccon->DebugOn = 0;
		break;

		case CtrIoctlGET_SW_DEBUG:
			*lap = ccon->DebugOn;
		break;

		case CtrIoctlGET_VERSION:
			ver = (CtrDrvrVersion *) arb;
			GetVersion(mcon,ver);
		break;

		case CtrIoctlSET_TIMEOUT:
			ccon->Timeout = lav;
		break;

		case CtrIoctlGET_TIMEOUT:
			*lap = ccon->Timeout;
		break;

		case CtrIoctlSET_QUEUE_FLAG:
			ccon->Queue.QueueOff = !!lav;
		break;

		case CtrIoctlGET_QUEUE_FLAG:
			*lap = ccon->Queue.QueueOff;
		break;

		case CtrIoctlGET_QUEUE_SIZE:
			*lap = ccon->Queue.Size;
		break;

		case CtrIoctlGET_QUEUE_OVERFLOW:
			*lap = ccon->Queue.Missed;
			ccon->Queue.Missed = 0;
		break;

		case CtrIoctlSET_MODULE_BY_SLOT:
			for (i=0; i<Wa.Modules; i++) {
				mcon = &(Wa.ModuleContexts[i]);
				if (mcon->PciSlot == lav) {
					ccon->ModuleIndex = i;
					break;
				}
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_MODULE_DESCRIPTOR:
			moad = (CtrDrvrModuleAddress *) arb;

			moad->ModuleType   = CtrDrvrModuleTypeCTR;
			moad->DeviceId     = CTRP_DEVICE_ID;
			moad->VendorId     = CERN_VENDOR_ID;
			moad->MemoryMap    = (uint32_t *) mcon->Map;
			moad->LocalMap     = (uint32_t *) mcon->Local;
			moad->ModuleNumber = mcon->ModuleIndex +1;
			moad->PciSlot      = mcon->PciSlot;
		break;

		case CtrIoctlGET_MODULE_SLOT:
			*lap = mcon->PciSlot;
		break;

		case CtrIoctlSET_MODULE:
			if ((lav >= 1)
			&&  (lav <= Wa.Modules)) {
				ccon->ModuleIndex = lav -1;
				mcon = &(Wa.ModuleContexts[ccon->ModuleIndex]);
				mcon->ModuleIndex = ccon->ModuleIndex;
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_MODULE:
			*lap = ccon->ModuleIndex +1;
		break;

		case CtrIoctlGET_MODULE_COUNT:
			*lap = Wa.Modules;
		break;

		case CtrIoctlLOCK:
			mcon->UpLock = 1;     /** Lock driver update commands */
		break;

		case CtrIoctlUNLOCK:
			clr_uplock(mcon,lav); /** To unlock you need the password !! */
		break;                        /** Never returns an error */

		case CtrIoctlRESET:
			if (mcon->UpLock) { cc=-EACCES; break; } /** Check lock before doing reset */
			Reset(mcon);
		break;

		case CtrIoctlGET_STATUS:
			*lap = GetStatus(mcon);
		break;

		case CtrIoctlGET_CLIENT_LIST:
			cls = (CtrDrvrClientList *) arb;
			memset(cls,0,sizeof(CtrDrvrClientList));

			for (i=0; i<CtrDrvrCLIENT_CONTEXTS; i++) {
				ccon = &(Wa.ClientContexts[i]);
				if (ccon->InUse)
					cls->Pid[cls->Size++] = ccon->Pid;
			}
		break;

		case CtrIoctlCONNECT:
			if (mcon->UpLock) { cc=-EACCES; break; }
			conx = (CtrDrvrConnection *) arb;
			cc = Connect(conx,ccon);
		break;

		case CtrIoctlDISCONNECT:
			conx = (CtrDrvrConnection *) arb;
			if (conx->EqpNum)
				cc = DisConnect(conx,ccon);
			else
				cc = DisConnectAll(ccon);
		break;

		case CtrIoctlGET_CLIENT_CONNECTIONS:
			ccn = (CtrDrvrClientConnections *) arb;
			ccn->Size = 0;

			for (i=0; i<CtrDrvrCLIENT_CONTEXTS; i++) {
				ccon = &(Wa.ClientContexts[i]);
				if ((ccon->InUse) && (ccon->Pid == ccn->Pid)) {
					msk = 1 << ccon->ClientIndex;
					for (j=0; j<CtrDrvrMODULE_CONTEXTS; j++) {
						mcon = &(Wa.ModuleContexts[j]);
						for (k=0; k<CtrDrvrRamTableSIZE; k++) {
							if (msk & mcon->Clients[k]) {
								found = 0;
								for (n=0; n<ccn->Size; n++) {
									conx = &(ccn->Connections[n]);
									if ((mcon->EqpNum[k]   == conx->EqpNum)
									&&  (mcon->EqpClass[k] == conx->EqpClass)) {
										found = 1;
										break;
									}
								}
								if (!found) {
									ccn->Connections[ccn->Size].Module   = mcon->ModuleIndex +1;
									ccn->Connections[ccn->Size].EqpNum   = mcon->EqpNum[k];
									ccn->Connections[ccn->Size].EqpClass = mcon->EqpClass[k];
									if (ccn->Size++ >= CtrDrvrCONNECTIONS)
										goto out;
								}
							}
						}

						hmsk = 0;
						for (k=0; k<CtrDrvrInterruptSOURCES; k++) {
							if (msk & mcon->HardClients[k])
								hmsk |= (1 << k);
							if (hmsk) {
								ccn->Connections[ccn->Size].Module   = mcon->ModuleIndex +1;
								ccn->Connections[ccn->Size].EqpNum   = hmsk;
								ccn->Connections[ccn->Size].EqpClass = CtrDrvrConnectionClassHARD;
								if (ccn->Size++ >= CtrDrvrCONNECTIONS)
									goto out;
							}
						}
					}
				}
			}
		break;

		case CtrIoctlSET_UTC:
			cc = SetTime(mcon,(uint32_t) lav);
		break;

		case CtrIoctlGET_UTC:
			ctod = (CtrDrvrCTime *) arb;
			*ctod = *GetTime(mcon);
		break;

		case CtrIoctlGET_CABLE_ID:
			if (mcon->CableId)
				*lap = mcon->CableId;
			else
				*lap = ioread32be(&mmap->CableId);
		break;

		case CtrIoctlSET_CABLE_ID:
			mcon->CableId = lav;
		break;

		case CtrIoctlHPTDC_OPEN:
			if (mcon->UpLock) { cc=-EACCES; break; }
			mcon->HptdcOpen = 1;
		break;

		case CtrIoctlHPTDC_IO:
			hpio = (CtrDrvrHptdcIoBuf *) arb;
			if (mcon->HptdcOpen) {
				cc = HptdcCommand(mcon,hpio->Cmd,hpio->Wreg,hpio->Rreg,hpio->Size,hpio->Pflg,hpio->Rflg);
				break;
			}
			cc = -ENOLCK;
		break;

		case CtrIoctlHPTDC_CLOSE:
			mcon->HptdcOpen = 0;
		break;

		case CtrIoctlRAW_READ:
			riob = (CtrDrvrRawIoBlock *) arb;

			iobuf = kmalloc(riob->Size * sizeof(uint32_t),GFP_KERNEL);
			if (!iobuf) {
				cc = -ENOMEM;
				break;
			}
			RawIo(mcon,iobuf,riob->Size,riob->Offset,0);

			if (copy_to_user(riob->UserArray,iobuf,riob->Size * sizeof(uint32_t)))
				cc = -EACCES;

			kfree(iobuf);
		break;

		case CtrIoctlRAW_WRITE:
			if (mcon->UpLock) { cc=-EACCES; break; }
			riob = (CtrDrvrRawIoBlock *) arb;

			iobuf = kmalloc(riob->Size * sizeof(uint32_t),GFP_KERNEL);
			if (!iobuf) {
				cc = -ENOMEM;
				break;
			}

			if (copy_from_user(iobuf,riob->UserArray,riob->Size * sizeof(uint32_t)))
				cc = -EACCES;
			else
				RawIo(mcon,iobuf,riob->Size,riob->Offset,1);

			kfree(iobuf);
		break;

		case CtrIoctlGET_ACTION:
			act = (CtrDrvrAction *) arb;
			if ((act->TriggerNumber > 0) && (act->TriggerNumber <= CtrDrvrRamTableSIZE)) {
				i = act->TriggerNumber -1;
				Int32Copy((uint32_t *) &(act->Trigger),
					  (uint32_t *) HardToTrigger(&(mmap->Trigs[i])),
					  (uint32_t  ) sizeof(CtrDrvrTrigger));
				Int32Copy((uint32_t *) &(act->Config ),
					  (uint32_t *) HardToConfig(&(mmap->Configs[i])),
					  (uint32_t  ) sizeof(CtrDrvrCounterConfiguration));
				act->Trigger.Ctim = mcon->Trigs[i].Ctim;
				act->EqpNum       = mcon->EqpNum[i];
				act->EqpClass     = mcon->EqpClass[i];

				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlSET_ACTION:
			if (mcon->UpLock) { cc=-EACCES; break; }
			act = (CtrDrvrAction *) arb;

			if ((act->TriggerNumber > 0) && (act->TriggerNumber <= CtrDrvrRamTableSIZE)) {
				i = act->TriggerNumber -1;

				if (mcon->EqpClass[i] == CtrDrvrConnectionClassHARD)
					break;

				if (mcon->EqpNum[i] !=0) {
					if (mcon->EqpNum[i] != act->EqpNum) {
						printk("ctr_ioctl: SetAction: Illegal EqpNum: %d->%d\n",
						      (int) mcon->EqpNum[i],
						      (int) act->EqpNum);
						cc = -EACCES;
						break;
					}

					if (mcon->EqpClass[i] != act->EqpClass) {
						printk("ctr_ioctl: SetAction: Illegal EqpClass: %d->%d\n",
						       (int) mcon->EqpClass[i],
						       (int) act->EqpClass);
						cc = -EACCES;
						break;
					}

					if (mcon->Trigs[i].Counter != act->Trigger.Counter) {
						printk("ctr_ioctl: SetAction: Illegal Counter: %d->%d\n",
						       (int) mcon->Trigs[i].Counter,
						       (int) act->Trigger.Counter);
						cc = -EACCES;
						break;
					}

				} else {
					mcon->EqpNum[i] = act->EqpNum;
					mcon->EqpClass[i] = act->EqpClass;
					mcon->Trigs[i].Counter = act->Trigger.Counter;
				}

				Io32Write((uint32_t *) &(mmap->Trigs[i]),
					  (uint32_t *) TriggerToHard(&act->Trigger),
					  (uint32_t  ) sizeof(CtrDrvrHwTrigger));
				mcon->Trigs[i] = act->Trigger;

				/* Override bus interrupt settings for connected clients */

				if (mcon->Clients[i])
					act->Config.OnZero |= CtrDrvrCounterOnZeroBUS;
				else
					act->Config.OnZero &= ~CtrDrvrCounterOnZeroBUS;

				Io32Write((uint32_t *) &(mmap->Configs[i]),
					  (uint32_t *) ConfigToHard(&(act->Config)),
					  (uint32_t  ) sizeof(CtrDrvrHwCounterConfiguration));
				mcon->Configs[i] = act->Config;

				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlCREATE_CTIM_OBJECT:
			if (mcon->UpLock) { cc=-EACCES; break; }
			ctim = (CtrDrvrCtimBinding *) arb;

			for (i=0; i<Wa.Ctim.Size; i++) {
				if (ctim->EqpNum == Wa.Ctim.Objects[i].EqpNum) {
					cc = -EBUSY;
					goto out;
				}
			}

			i = Wa.Ctim.Size;
			if (i<CtrDrvrCtimOBJECTS) {
				Wa.Ctim.Objects[i] = *ctim;
				Wa.Ctim.Size = i +1;
				break;;
			}
			cc = -ENOMEM;
		break;

		case CtrIoctlDESTROY_CTIM_OBJECT:
			if (mcon->UpLock) { cc=-EACCES; break; }
			ctim = (CtrDrvrCtimBinding *) arb;

			for (i=0; i<Wa.Ctim.Size; i++) {
				if (ctim->EqpNum == Wa.Ctim.Objects[i].EqpNum) {
					for (j=0; j<CtrDrvrRamTableSIZE; j++) {
						strg = &(mcon->Trigs[j]);
						if (strg->Ctim == ctim->EqpNum) {
							cc = -EBUSY;
							goto out;
						}
					}
					Wa.Ctim.Objects[i] = Wa.Ctim.Objects[Wa.Ctim.Size -1];
					Wa.Ctim.Size--;
					goto out;
				}
			}
		break;

		case CtrIoctlCHANGE_CTIM_FRAME:
			if (mcon->UpLock) { cc=-EACCES; break; }
			ctim = (CtrDrvrCtimBinding *) arb;

			for (i=0; i<Wa.Ctim.Size; i++) {
				if (ctim->EqpNum == Wa.Ctim.Objects[i].EqpNum) {
					for (j=0; j<CtrDrvrRamTableSIZE; j++) {
						strg = &(mcon->Trigs[j]);
						if (strg->Ctim == ctim->EqpNum) {
							cc = -EBUSY;
							goto out;
						}
					}
					Wa.Ctim.Objects[i].Frame.Long = ctim->Frame.Long;
					goto out;
				}
			}
			cc = -EINVAL;
		break;

		case CtrIoctlLIST_CTIM_OBJECTS:
			ctimo = (CtrDrvrCtimObjects *) arb;
			memcpy(ctimo,&(Wa.Ctim),sizeof(CtrDrvrCtimObjects));
		break;

		case CtrIoctlCREATE_PTIM_OBJECT:     /* Create a new PTIM timing object */
			if (mcon->UpLock) { cc=-EACCES; break; }
			ptim = (CtrDrvrPtimBinding *) arb;

			for (i=0; i<Wa.Ptim.Size; i++) {
				if (ptim->EqpNum == Wa.Ptim.Objects[i].EqpNum) {
					cc = -EBUSY;
					goto out;
				}
			}
			if (Wa.Ptim.Size >= CtrDrvrPtimOBJECTS) {
				cc = -ENOMEM;
				break;
			}

			if (ptim->ModuleIndex < Wa.Modules) {
				mcon = &(Wa.ModuleContexts[ptim->ModuleIndex]);

				/* Look for available space in trigger table for specified module */

				found = size = start = 0;
				for (i=0; i<CtrDrvrRamTableSIZE; i++) {
					if (mcon->EqpNum[i] == 0) {
						if (size == 0)
							start = i;
						if (++size >= ptim->Size) {
							found = 1;
							break;
						}
					} else
						size = 0;
				}

				if (found) {
					for (i=start; i<start+ptim->Size; i++) {
						mcon->EqpNum[i]   = ptim->EqpNum;
						mcon->EqpClass[i] = CtrDrvrConnectionClassPTIM;
						mcon->Trigs[i].Counter = ptim->Counter;
					}

					ptim->StartIndex = start;
					Wa.Ptim.Objects[Wa.Ptim.Size++] = *ptim;
					break;
				}
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_PTIM_BINDING:     /* Search for a PTIM object binding */
			ptim = (CtrDrvrPtimBinding *) arb;
			for (i=0; i<Wa.Ptim.Size; i++) {
				if (ptim->EqpNum == Wa.Ptim.Objects[i].EqpNum) {
					*ptim = Wa.Ptim.Objects[i];
					goto out;
				}
			}
			cc = -EINVAL;
		break;

		case CtrIoctlDESTROY_PTIM_OBJECT:    /* Destroy a PTIM timing object */
			if (mcon->UpLock) { cc=-EACCES; break; }
			ptim = (CtrDrvrPtimBinding *) arb;

			i = ptim->ModuleIndex;
			if ((i < 0) || (i >= Wa.Modules)) {
			       cc = -EINVAL;
			       break;
			}
			mcon = &(Wa.ModuleContexts[ptim->ModuleIndex]);
			mmap = (CtrDrvrMemoryMap *) mcon->Map;

			found = 0;
			for (i=0; i<CtrDrvrRamTableSIZE; i++) {
				if ((mcon->EqpNum[i]   == ptim->EqpNum)
				&&  (mcon->EqpClass[i] == CtrDrvrConnectionClassPTIM)) {
					if (mcon->Clients[i]) {
						cc = -EBUSY;
						goto out;
					}
					found = 1;
				}
			}

			if (found) {
				for (i=0; i<CtrDrvrRamTableSIZE; i++) {
					if ((mcon->EqpNum[i]   == ptim->EqpNum)
					&&  (mcon->EqpClass[i] == CtrDrvrConnectionClassPTIM)) {

						htrg = &((CtrDrvrHwTrigger) {{0},0});
						Io32Write((uint32_t *) &(mmap->Trigs[i]),
							  (uint32_t *) htrg,
							  (uint32_t  ) sizeof(CtrDrvrHwTrigger));

						mcon->EqpNum[i]   = 0;
						mcon->EqpClass[i] = 0;
						memset(&(mcon->Configs[i]),0,sizeof(CtrDrvrCounterConfiguration));
						memset(&(mcon->Trigs[i]),0,sizeof(CtrDrvrTrigger));
						Io32Write((uint32_t *) &(mmap->Configs[i]),
							  (uint32_t *) ConfigToHard(&(mcon->Configs[i])),
							  (uint32_t  ) sizeof(CtrDrvrHwCounterConfiguration));
					}
				}

				for (i=0; i<Wa.Ptim.Size; i++) {
					if (ptim->EqpNum == Wa.Ptim.Objects[i].EqpNum) {
						Wa.Ptim.Objects[i] = Wa.Ptim.Objects[Wa.Ptim.Size -1];
						Wa.Ptim.Size--;
						break;
					}
				}
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlLIST_PTIM_OBJECTS:
			ptimo = (CtrDrvrPtimObjects *) arb;
			memcpy(ptimo,&(Wa.Ptim),sizeof(CtrDrvrPtimObjects));
		break;

		case CtrIoctlENABLE:
			if (mcon->UpLock) { cc=-EACCES; break; }

			if        (lav   == CtrDrvrCommandSET_HPTDC) {
				mcon->Command |=  CtrDrvrCommandSET_HPTDC;
				iowrite32be(mcon->Command,&mmap->Command);
			} else if (lav   == ~CtrDrvrCommandSET_HPTDC) {
				mcon->Command &= ~CtrDrvrCommandSET_HPTDC;
				iowrite32be(mcon->Command,&mmap->Command);
			} else if (lav   != 0) {
				mcon->Command |=  CtrDrvrCommandENABLE;
				mcon->Command &= ~CtrDrvrCommandDISABLE;
				iowrite32be(mcon->Command,&mmap->Command);
			} else {
				mcon->Command &= ~CtrDrvrCommandENABLE;;
				mcon->Command |=  CtrDrvrCommandDISABLE;
				iowrite32be(mcon->Command,&mmap->Command);
			}
		break;

		case CtrIoctlSET_DEBUG_HISTORY:
			if (mcon->UpLock) { cc=-EACCES; break; }

			if (lav) {
				mcon->Command |=  CtrDrvrCommandDebugHisOn;
				mcon->Command &= ~CtrDrvrCommandDebugHisOff;
			} else {
				mcon->Command &= ~CtrDrvrCommandDebugHisOn;
				mcon->Command |=  CtrDrvrCommandDebugHisOff;
			}
			iowrite32be(mcon->Command,&mmap->Command);
		break;

		case CtrIoctlSET_BRUTAL_PLL:
			if (mcon->UpLock) { cc=-EACCES; break; }

			if (lav) {
				mcon->Command |=  CtrDrvrCommandUtcPllOff;
				mcon->Command &= ~CtrDrvrCommandUtcPllOn;
			} else {
				mcon->Command &= ~CtrDrvrCommandUtcPllOff;
				mcon->Command |=  CtrDrvrCommandUtcPllOn;
			}
			iowrite32be(mcon->Command,&mmap->Command);
		break;

		case CtrIoctlGET_INPUT_DELAY:
			*lap = ioread32be(&mmap->InputDelay);
		break;

		case CtrIoctlSET_INPUT_DELAY:
			if (mcon->UpLock) { cc=-EACCES; break; }

			mcon->InputDelay = lav;
			iowrite32be(lav,&mmap->InputDelay);
		break;

		case CtrIoctlGET_REMOTE:
			remc = (CtrdrvrRemoteCommandBuf *) arb;
			if ((remc->Counter >= CtrDrvrCounter1) && (remc->Counter <= CtrDrvrCounter8)) {
				remc->Remote = ioread32be(&mmap->Counters[remc->Counter].Control.LockConfig);
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlSET_REMOTE:
			if (mcon->UpLock) { cc=-EACCES; break; }
			remc = (CtrdrvrRemoteCommandBuf *) arb;

			if ((remc->Counter >= CtrDrvrCounter1) && (remc->Counter <= CtrDrvrCounter8)) {
				if (remc->Remote)
					iowrite32be(1,&mmap->Counters[remc->Counter].Control.LockConfig);
				else
					iowrite32be(0,&mmap->Counters[remc->Counter].Control.LockConfig);
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlREMOTE:
			if (mcon->UpLock) { cc=-EACCES; break; }
			remc = (CtrdrvrRemoteCommandBuf *) arb;

			if ((remc->Counter >= CtrDrvrCounter1) && (remc->Counter <= CtrDrvrCounter8)) {
				if (ioread32be(&mmap->Counters[remc->Counter].Control.LockConfig)) {
					iowrite32be(CtrDrvrRemoteEXEC | (CtrDrvrCntrCntrlREMOTE_MASK & remc->Remote),
						    &mmap->Counters[remc->Counter].Control.RemOutMask);
					break;
				}
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_MODULE_STATS:
			mods = (CtrDrvrModuleStats *) arb;
			Io32Read((uint32_t *) mods,
				 (uint32_t *) &(mmap->ModStats),
				 (uint32_t  ) sizeof(CtrDrvrModuleStats));
		break;

		case CtrIoctlGET_OUT_MASK:
			cmsb = (CtrDrvrCounterMaskBuf *) arb;
			if ((cmsb->Counter >= CtrDrvrCounter1) && (cmsb->Counter <= CtrDrvrCounter8)) {
				i = cmsb->Counter;
				msk = ioread32be(&mmap->Counters[i].Control.RemOutMask);
				if (CtrDrvrCntrCntrlTTL_BAR & msk)
					cmsb->Polarity = CtrDrvrPolarityTTL_BAR;
				else
					cmsb->Polarity = CtrDrvrPolarityTTL;
				msk &= ~CtrDrvrCntrCntrlTTL_BAR;
				cmsb->Mask = AutoShiftRight(CtrDrvrCntrCntrlOUT_MASK,msk);
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlSET_OUT_MASK:
			if (mcon->UpLock) { cc=-EACCES; break; }
			cmsb = (CtrDrvrCounterMaskBuf *) arb;

			if ((cmsb->Counter >= CtrDrvrCounter1) && (cmsb->Counter <= CtrDrvrCounter8)) {
				i = cmsb->Counter;
				if (cmsb->Mask == 0)
					cmsb->Mask = 1 << cmsb->Counter;
				if (cmsb->Polarity == CtrDrvrPolarityTTL_BAR)
					msk = CtrDrvrCntrCntrlTTL_BAR;
				else
					msk = 0;
				iowrite32be(AutoShiftLeft(CtrDrvrCntrCntrlOUT_MASK,cmsb->Mask) | msk,
					    &mmap->Counters[i].Control.RemOutMask);
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_CONFIG:
			conf = (CtrDrvrCounterConfigurationBuf *) arb;
			if ((conf->Counter >= CtrDrvrCounter0) && (conf->Counter <= CtrDrvrCounter8)) {
				Int32Copy((uint32_t *) &(conf->Config),
					  (uint32_t *) HardToConfig(&mmap->Counters[conf->Counter].Config),
					  (uint32_t  ) sizeof(CtrDrvrCounterConfiguration));

				if ((conf->Config.OnZero > (CtrDrvrCounterOnZeroBUS | CtrDrvrCounterOnZeroOUT))
				||  (conf->Config.Start  > CtrDrvrCounterSTARTS)
				||  (conf->Config.Mode   > CtrDrvrCounterMODES)
				||  (conf->Config.Clock  > CtrDrvrCounterCLOCKS)) {
					cc = -EFAULT;
					break;
				}
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlSET_CONFIG:
			if (mcon->UpLock) { cc=-EACCES; break; }
			conf = (CtrDrvrCounterConfigurationBuf *) arb;

			if (mmap->Counters[conf->Counter].Control.LockConfig) {
				if ((conf->Counter >= CtrDrvrCounter0) && (conf->Counter <= CtrDrvrCounter8)) {
					Io32Write((uint32_t *) &(mmap->Counters[conf->Counter].Config),
						  (uint32_t *) ConfigToHard(&conf->Config),
						  (uint32_t  ) sizeof(CtrDrvrHwCounterConfiguration));
					break;
				}
			}
		break;

		case CtrIoctlGET_COUNTER_HISTORY:
			hisb = (CtrDrvrCounterHistoryBuf *) arb;
			if ((hisb->Counter >= CtrDrvrCounter1) && (hisb->Counter <= CtrDrvrCounter8)) {
				Io32Read((uint32_t *) &(hisb->History),
					 (uint32_t *) &(mmap->Counters[hisb->Counter].History),
					 (uint32_t  ) sizeof(CtrDrvrCounterHistory));
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlGET_PLL:
			pll = (CtrDrvrPll *) arb;
			Io32Read((uint32_t *) pll,
				 (uint32_t *) &(mmap->Pll),
				 (uint32_t  ) sizeof(CtrDrvrPll));
		break;

		case CtrIoctlSET_PLL:
			if (mcon->UpLock) { cc=-EACCES; break; }

			pll = (CtrDrvrPll *) arb;
			Io32Write((uint32_t *) &(mmap->Pll),
				  (uint32_t *) pll,
				  (uint32_t  ) sizeof(CtrDrvrPll));
			Int32Copy((uint32_t *) &(mcon->Pll),
				  (uint32_t *) pll,
				  (uint32_t  ) sizeof(CtrDrvrPll));
		break;

		case CtrIoctlGET_PLL_ASYNC_PERIOD:   /* Get PLL asynchronous period */
			asyp = (CtrDrvrPllAsyncPeriodNs *) arb;
			if (mcon->PllAsyncPeriodNs != 0.0)
				*asyp = mcon->PllAsyncPeriodNs;
			else
				*asyp = mcon->PllAsyncPeriodNs = 1000.00/40.000;
		break;

		case CtrIoctlSET_PLL_ASYNC_PERIOD:
			if (mcon->UpLock) { cc=-EACCES; break; }
			asyp = (CtrDrvrPllAsyncPeriodNs *) arb;

			*asyp = mcon->PllAsyncPeriodNs;
		break;

		case CtrIoctlREAD_TELEGRAM:
			tgmb = (CtrDrvrTgmBuf *) arb;
			if ((tgmb->Machine > CtrDrvrMachineNONE) && (tgmb->Machine <= CtrDrvrMachineMACHINES)) {
				Io32Read((uint32_t *) tgmb->Telegram,
					 (uint32_t *) mmap->Telegrams[(int) (tgmb->Machine) -1],
					 (uint32_t  ) sizeof(CtrDrvrTgm));
				SwapWords((uint32_t *) tgmb->Telegram, (uint32_t  ) sizeof(CtrDrvrTgm));
				break;
			}
			cc = -EINVAL;
		break;

		case CtrIoctlREAD_EVENT_HISTORY:
			evhs = (CtrDrvrEventHistoryBuf *) arb;
			ReadEventHistory(mcon, evhs);
		break;

		case CtrIoctlGET_RECEPTION_ERRORS:
			rcpe = (CtrDrvrReceptionErrors *) arb;
			rcpe->LastReset    = ioread32be(&mmap->LastReset);
			rcpe->PartityErrs  = ioread32be(&mmap->PartityErrs);
			rcpe->SyncErrs     = ioread32be(&mmap->SyncErrs);
			rcpe->TotalErrs    = ioread32be(&mmap->TotalErrs);
			rcpe->CodeViolErrs = ioread32be(&mmap->CodeViolErrs);
			rcpe->QueueErrs    = ioread32be(&mmap->QueueErrs);
		break;

		case CtrIoctlGET_IO_STATUS:
			*lap = ioread32be(&mmap->IoStat);
		break;

		case CtrIoctlGET_IDENTITY:
			bird = (CtrDrvrBoardId *) arb;
			bird->IdLSL = ioread32be(&mmap->IdLSL);
			bird->IdMSL = ioread32be(&mmap->IdMSL);
		break;

		default:
			cc = -ENOENT;
	}

out:
	debug_ioctl(ccon->DebugOn, iodr, iosz, ionr, arb, 0);

	if ((arg) && (iodr & _IOC_READ) && copy_to_user((void *) arg, arb, iosz))
		cc = -EACCES;
	kfree(arb);
	return cc;
}

long ctr_ioctl(struct file *filp, uint32_t cmd, unsigned long arg)
{
	int cc;
	mutex_lock(&ctr_drvr_mutex);
	cc = __ctr_ioctl(filp,cmd,arg);
	mutex_unlock(&ctr_drvr_mutex);
	return cc;
}

struct file_operations ctr_fops = {
   .read           = ctr_read,
   .write          = ctr_write,
   .unlocked_ioctl = ctr_ioctl,
   .open           = ctr_open,
   .release        = ctr_close,
};

module_init(ctr_install);
module_exit(ctr_uninstall);
