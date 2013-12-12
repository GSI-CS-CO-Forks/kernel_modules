/* ********************************************************************************************* */
/*                                                                                               */
/* CTR (Controls Timing Receiver) Private include file containing driver structures.             */
/*                                                                                               */
/* Julian Lewis 26th Aug 2003                                                                    */
/*                                                                                               */
/* ********************************************************************************************* */

#include "ctrhard.h"
#include "ctrdrvr.h"

#ifndef CTR_PRIVATE
#define CTR_PRIVATE

#ifdef CTR_PCI
#include "plx9030.h"

#define CERN_VENDOR_ID 0x10DC
#define CTRP_DEVICE_ID 0x0300

/* ============================================================ */
/* Mapping of JTAG pins onto plx9030 GPIOC                      */
/* ============================================================ */

#define CtrDrvrJTAG_PIN_TMS_ONE  0x4
#define CtrDrvrJTAG_PIN_TMS_OUT  0x2
#define CtrDrvrJTAG_PIN_TCK_ONE  0x20
#define CtrDrvrJTAG_PIN_TCK_OUT  0x10
#define CtrDrvrJTAG_PIN_TDI_ONE  0x100
#define CtrDrvrJTAG_PIN_TDI_OUT  0x80
#define CtrDrvrJTAG_PIN_TDO_ONE  0x800  /* These are inputs to the PLX */
#define CtrDrvrJTAG_PIN_TDO_OUT  0x400  /* Never set this bit */

/* ============================================================ */
/* Endian is needed to set up the Plx9030 mapping               */
/* ============================================================ */

typedef enum {
   CtrDrvrEndianUNKNOWN,
   CtrDrvrEndianBIG,
   CtrDrvrEndianLITTLE
 } CtrDrvrEndian;
#endif

/* ============================================================ */
/* The client context                                           */
/* ============================================================ */

#define CtrDrvrDEFAULT_TIMEOUT 2000 /* In 10 ms ticks */
#define CtrDrvrQUEUE_SIZE 64        /* Maximum queue size */

typedef struct {
   uint16_t       QueueOff;
   uint16_t       Missed;
   uint16_t       Size;
   uint32_t       RdPntr;
   uint32_t       WrPntr;
   CtrDrvrReadBuf Entries[CtrDrvrQUEUE_SIZE];
 } CtrDrvrQueue;

typedef struct {
   uint32_t          DebugOn;
   uint32_t          Pid;
   uint32_t          ModuleIndex;
   uint32_t          ClientIndex;
   uint32_t          Timeout;
   uint32_t          InUse;
   spinlock_t        Lock;
   wait_queue_head_t Wq;
   CtrDrvrQueue      Queue;
 } CtrDrvrClientContext;

/* ============================================================ */
/* The Module context                                           */
/* ============================================================ */

#define CtrDrvrMAX_BUS_ERROR_COUNT 100

typedef struct {

#ifdef CTR_PCI
   struct pci_dev             *dev;               /* PCI device for this module */
   uint32_t                   *Local;             /* Plx9030 local config space */
   uint32_t                    PciSlot;           /* Module geographic ID PCI Slot */
#endif

#ifdef CTR_VME
   CtrDrvrModuleAddress        Address;           /* Vme address */
   uint32_t                    OutputByte;        /* Output byte number 1..64 */
#endif

   CtrDrvrMemoryMap           *Map;               /* Pointer to the real hardware */
   uint32_t                    HptdcOpen;         /* Hptdc chip configuration open */
   uint32_t                    ModuleIndex;       /* Which module we are */
   uint32_t                    Command;           /* Command word */
   uint32_t                    InterruptEnable;   /* Enabled interrupts mask */
   CtrDrvrPllAsyncPeriodNs     PllAsyncPeriodNs;  /* Pll async period */
   uint32_t                    InputDelay;        /* Specified in 40MHz ticks */
   CtrDrvrPll                  Pll;               /* PLL parameters */
   uint32_t                    Status;            /* Module status */
   uint32_t                    InUse;             /* Module context in use */
   uint32_t                    BusErrorCount;     /* Abort fatal loops with this */
   uint32_t                    CableId;           /* Frig a cable ID for module */
   uint32_t                    UpLock;            /* Update lock */

   CtrDrvrCounterConfiguration Configs [CtrDrvrRamTableSIZE];
   uint32_t                    Clients [CtrDrvrRamTableSIZE];      /* Clients interrupts PTIM/CTIM */
   CtrDrvrTrigger              Trigs   [CtrDrvrRamTableSIZE];
   uint32_t                    EqpNum  [CtrDrvrRamTableSIZE];
   CtrDrvrConnectionClass      EqpClass[CtrDrvrRamTableSIZE];

   uint32_t                    HardClients[CtrDrvrInterruptSOURCES];   /* Clients interrupts HARD */

 } CtrDrvrModuleContext;

/* ============================================================ */
/* Driver Working Area                                          */
/* ============================================================ */

typedef struct {
   CtrDrvrCtimObjects   Ctim;
   CtrDrvrPtimObjects   Ptim;
   uint32_t             Modules;
   CtrDrvrModuleContext ModuleContexts[CtrDrvrMODULE_CONTEXTS];
   CtrDrvrClientContext ClientContexts[CtrDrvrCLIENT_CONTEXTS];
 } CtrDrvrWorkingArea;

#endif
