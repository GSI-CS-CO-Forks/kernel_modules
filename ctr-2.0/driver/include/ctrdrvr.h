/* ********************************************************************************************* */
/*                                                                                               */
/* CTR (Controls Timing Receiver PMC) Public include file containing driver structures, that     */
/* are used by client tasks.                                                                     */
/*                                                                                               */
/* Julian Lewis 19th May 2003                                                                    */
/*                                                                                               */
/* ********************************************************************************************* */

#ifndef CTRDRVR
#define CTRDRVR

#ifndef __kernel
#include <stdint.h>
#endif

#ifndef uintptr_t
#define uintptr_t unsigned long
#endif

#include "ctrhard.h"   /* Hardware interface for expert and diagnostic clients */
#include "hptdc.h"     /* HPTDC (Time to Digital Convertor) definitions */

/* Maximum number of simultaneous clients for driver */

#define CtrDrvrCLIENT_CONTEXTS 16

/* Maximum number of CTR modules on one host processor */

#define CtrDrvrMODULE_CONTEXTS 16

/* *********************************************************************************** */
/* In some rare cases, it is necessary to distinguish between CTR device types.        */
/* This can control the open and install routines so that more than one device type    */
/* can be used at the sam time on a single host.                                       */

typedef enum {
   CtrDrvrDeviceANY,    /* Any CTR device, don't care which */
   CtrDrvrDeviceCTRI,   /* CTR PCI type */
   CtrDrvrDeviceCTRP,   /* CTR PMC type */
   CtrDrvrDeviceCTRV,   /* CTR VME type */
   CtrDrvrDeviceCTRE,   /* CTR VME modified for Beam Energy Safe Beam Parameters */
   CtrDrvrDEVICES
 } CtrDrvrDevice;

/* *********************************************************************************** */
/* All client access to the ctrp timing receivers is through the concept of the timing */
/* object, as per the equipment module classes PTIM, and CTIM. Event frame layouts are */
/* usually hidden from normal clients. They are available in the ctrphard.h interface. */

#define CtrDrvrCONNECTIONS 512

typedef enum {
   CtrDrvrConnectionClassHARD, /* Used when connecting to CTR hardware interrupt  */
   CtrDrvrConnectionClassCTIM, /* Used when connecting to incomming timing on GMT */
   CtrDrvrConnectionClassPTIM  /* Used when connecting to a local PTIM timing     */
 } CtrDrvrConnectionClass;

typedef struct {
   uint32_t                Module;       /* The module 1..n */
   CtrDrvrConnectionClass  EqpClass;     /* Incomming CTIM or local PTIM timing */
   uint32_t                EqpNum;       /* Either a MASK or PTIM or CTIM object  */
 } CtrDrvrConnection;

typedef struct {
   uint32_t     Size;
   uint32_t     Pid[CtrDrvrCLIENT_CONTEXTS];
 } CtrDrvrClientList;

typedef struct {
   uint32_t           Pid;
   uint32_t           Size;
   CtrDrvrConnection  Connections[CtrDrvrCONNECTIONS];
 } CtrDrvrClientConnections;

typedef struct {
   CtrDrvrConnection Connection;       /* PTIM, CTIM or HARD object */
   uint32_t          TriggerNumber;    /* Trigger number 1..n */
   uint32_t          InterruptNumber;  /* 0,1..8 (Counter) 9..n Hardware */
   uint32_t          Ctim;             /* Ctim trigger */
   CtrDrvrEventFrame Frame;            /* Triggering event frame */
   CtrDrvrCTime      TriggerTime;      /* Time counter loaded */
   CtrDrvrCTime      StartTime;        /* Time counter was started */
   CtrDrvrCTime      OnZeroTime;       /* Time of interrupt */
 } CtrDrvrReadBuf;

typedef struct {
   uint32_t          TriggerNumber;    /* Trigger number 0..n 0=>First in connection */
   CtrDrvrConnection Connection;       /* Connection for trigger */
   uint32_t          Payload;          /* Used when simulating CTIMs */
 } CtrDrvrWriteBuf;

/* ******************************************************* */
/* Direct access to triggers and counter configurations    */
/* stored in the CTR ram.                                  */

typedef struct {
   uint32_t                    TriggerNumber;   /* 1..n    */
   uint32_t                    EqpNum;          /* Object  */
   CtrDrvrConnectionClass      EqpClass;        /* Class   */
   CtrDrvrTrigger              Trigger;         /* Trigger */
   CtrDrvrCounterConfiguration Config;          /* Counter */
 } CtrDrvrAction;

/* ******************************************************* */
/* CTIM object bindings                                    */

#define CtrDrvrCtimOBJECTS 2048

typedef struct {
   uint32_t          EqpNum;
   CtrDrvrEventFrame Frame;
 } CtrDrvrCtimBinding;

typedef struct {
   uint16_t           Size;
   CtrDrvrCtimBinding Objects[CtrDrvrCtimOBJECTS];
 } CtrDrvrCtimObjects;

/* ******************************************************* */
/* PTIM object bindings                                    */

#define CtrDrvrPtimOBJECTS 2048

typedef struct {
   uint32_t       EqpNum;
   uint8_t        ModuleIndex;
   uint8_t        Counter;
   uint16_t       Size;
   uint16_t       StartIndex;
 } CtrDrvrPtimBinding;

typedef struct {
   uint16_t           Size;
   CtrDrvrPtimBinding Objects[CtrDrvrPtimOBJECTS];
 } CtrDrvrPtimObjects;

/* ******************************************************* */
/* Event History buffer                                    */

#define CtrDrvrHISTORY_BUF_SIZE 512

typedef struct {
   uint32_t                 Index;
   CtrDrvrEventHistoryEntry Entries[CtrDrvrHISTORY_BUF_SIZE];
 } CtrDrvrEventHistoryBuf;

/* ***************************************************** */
/* Module descriptor                                     */

#define PCI_BUS 1
#define VME_BUS 2

typedef struct {
   uint32_t BusType;         /* PCI_BUS/VME_BUS else invalid */
   uint32_t HardwareAddress; /* PCI(Bus/Slot) or VME(BaseAddress) */
   uint32_t InterruptVector;
 } CtrDrvrModuleAddress;

/* ***************************************************** */
/* The compilation dates in UTC time for components.     */

typedef enum {
   CtrDrvrHardwareTypeNONE,
   CtrDrvrHardwareTypeCTRP,
   CtrDrvrHardwareTypeCTRI,
   CtrDrvrHardwareTypeCTRV
 } CtrDrvrHardwareType;

#define CtrDrvrHardwareTYPES 4

typedef struct {
   uint32_t            VhdlVersion;  /* VHDL Compile date */
   uint32_t            DrvrVersion;  /* Drvr Compile date */
   CtrDrvrHardwareType HardwareType; /* Hardware type of module */
 } CtrDrvrVersion;

/* ***************************************************** */
/* HPTDC IO buffer                                       */

typedef struct {
   uint32_t            Size;   /* Number of HPTDC regs   */
   uint32_t            Pflg;   /* Parity flag            */
   uint32_t            Rflg;   /* Reset state flag       */
   HptdcCmd            Cmd;    /* Command                */
   HptdcRegisterVlaue  *Wreg;  /* Write  to HPTDC buffer */
   HptdcRegisterVlaue  *Rreg;  /* Read from HPTDC buffer */
 } CtrDrvrHptdcIoBuf;

/* ***************************************************** */
/* Counter configuration IO buffer                       */

typedef struct {
   CtrDrvrCounter Counter;
   CtrDrvrCounterConfiguration Config;
 } CtrDrvrCounterConfigurationBuf;

/* ***************************************************** */
/* Counter history IO buffer                             */

typedef struct {
   CtrDrvrCounter Counter;
   CtrDrvrCounterHistory History;
 } CtrDrvrCounterHistoryBuf;

/* ***************************************************** */
/* Counter remote command IO buffer                      */

typedef struct {
   CtrDrvrCounter Counter;
   CtrDrvrRemote Remote;
 } CtrdrvrRemoteCommandBuf;

/* ***************************************************** */
/* Counter output mask IO buffer                         */

typedef enum {
   CtrDrvrPolarityTTL,
   CtrDrvrPolarityTTL_BAR
 } CtrDrvrPolarity;

typedef struct {
   CtrDrvrCounter Counter;
   CtrDrvrCounterMask Mask;
   CtrDrvrPolarity Polarity;
 } CtrDrvrCounterMaskBuf;

/* ***************************************************** */
/* Telegram IO buffer                                    */

typedef struct {
   CtrDrvrMachine Machine;
   CtrDrvrTgm Telegram;
 } CtrDrvrTgmBuf;

/* ***************************************************** */
/* Raw IO                                                */

typedef struct {
   uint32_t     Size;       /* Number int to read/write */
   uint32_t     Offset;     /* Long offset address space */
   uint32_t     *UserArray; /* Callers data area for  IO */
 } CtrDrvrRawIoBlock;

/* ***************************************************** */
/* Reception errors                                      */

typedef struct {
   uint32_t     LastReset;     /* UTC Second of last reset */
   uint32_t     PartityErrs;   /* Number of parity errors since last reset */
   uint32_t     SyncErrs;      /* Number of frame synchronization errors since last reset */
   uint32_t     TotalErrs;     /* Total number of IO errors since last reset */
   uint32_t     CodeViolErrs;  /* Number of code violations since last reset */
   uint32_t     QueueErrs;     /* Number of input Queue overflows since last reset */
 } CtrDrvrReceptionErrors;

/* ***************************************************** */
/* Board chip identity                                   */

typedef struct {
   uint32_t     IdLSL;         /* ID Chip value Least Sig 32-bits */
   uint32_t     IdMSL;         /* ID Chip value Most  Sig 32-bits */
 } CtrDrvrBoardId;

/* ***************************************************** */
/* Very special ISR debug code                           */

#define CtrDrvrDEBUG_ISR 0xFFFF

/* ***************************************************** */
/* Define the IOCTLs                                     */

typedef enum {

   /* Standard IOCTL Commands for timing users and developers */

   CtrDrvrSET_SW_DEBUG,           /* Set driver debug mode */
   CtrDrvrGET_SW_DEBUG,           /* Get driver debug mode */

   CtrDrvrGET_VERSION,            /* Get version date */

   CtrDrvrSET_TIMEOUT,            /* Set the read timeout value */
   CtrDrvrGET_TIMEOUT,            /* Get the read timeout value */

   CtrDrvrSET_QUEUE_FLAG,         /* Set queuing capabiulities on off */
   CtrDrvrGET_QUEUE_FLAG,         /* 1=Q_off 0=Q_on */
   CtrDrvrGET_QUEUE_SIZE,         /* Number of events on queue */
   CtrDrvrGET_QUEUE_OVERFLOW,     /* Number of missed events */

   CtrDrvrGET_MODULE_DESCRIPTOR,  /* Get the current Module descriptor */
   CtrDrvrSET_MODULE,             /* Select the module to work with */
   CtrDrvrGET_MODULE,             /* Which module am I working with */
   CtrDrvrGET_MODULE_COUNT,       /* The number of installed modules */

   CtrDrvrRESET,                  /* Reset the module, re-establish connections */
   CtrDrvrENABLE,                 /* Enable CTR module event reception */
   CtrDrvrGET_STATUS,             /* Read module status */

   CtrDrvrGET_INPUT_DELAY,        /* Get input delay in 25ns ticks */
   CtrDrvrSET_INPUT_DELAY,        /* Set input delay in 25ns ticks */

   CtrDrvrGET_CLIENT_LIST,        /* Get the list of driver clients */

   CtrDrvrCONNECT,                /* Connect to an object interrupt */
   CtrDrvrDISCONNECT,             /* Disconnect from an object interrupt */
   CtrDrvrGET_CLIENT_CONNECTIONS, /* Get the list of a client connections on module */

   CtrDrvrSET_UTC,                /* Set Universal Coordinated Time for next PPS tick */
   CtrDrvrGET_UTC,                /* Latch and read the current UTC time */

   CtrDrvrGET_CABLE_ID,           /* Cables telegram ID */

   CtrDrvrGET_ACTION,             /* Low level direct access to CTR RAM tables */
   CtrDrvrSET_ACTION,             /* Set may not modify the bus interrupt settings */

   CtrDrvrCREATE_CTIM_OBJECT,     /* Create a new CTIM timing object */
   CtrDrvrDESTROY_CTIM_OBJECT,    /* Destroy a CTIM timing object */
   CtrDrvrLIST_CTIM_OBJECTS,      /* Returns a list of created CTIM objects */
   CtrDrvrCHANGE_CTIM_FRAME,      /* Change the frame of an existing CTIM object */

   CtrDrvrCREATE_PTIM_OBJECT,     /* Create a new PTIM timing object */
   CtrDrvrDESTROY_PTIM_OBJECT,    /* Destroy a PTIM timing object */
   CtrDrvrLIST_PTIM_OBJECTS,      /* List PTIM timing objects */
   CtrDrvrGET_PTIM_BINDING,       /* Search for a PTIM object binding */

   CtrDrvrGET_OUT_MASK,           /* Counter output routing mask */
   CtrDrvrSET_OUT_MASK,           /* Counter output routing mask */
   CtrDrvrGET_COUNTER_HISTORY,    /* One deep history of counter */

   CtrDrvrGET_REMOTE,             /* Counter Remote/Local status */
   CtrDrvrSET_REMOTE,             /* Counter Remote/Local status */
   CtrDrvrREMOTE,                 /* Remote control counter */

   CtrDrvrGET_CONFIG,             /* Get a counter configuration */
   CtrDrvrSET_CONFIG,             /* Set a counter configuration */

   CtrDrvrGET_PLL,                /* Get phase locked loop parameters */
   CtrDrvrSET_PLL,                /* Set phase locked loop parameters */
   CtrDrvrSET_PLL_ASYNC_PERIOD,   /* Set PLL asynchronous period */
   CtrDrvrGET_PLL_ASYNC_PERIOD,   /* Get PLL asynchronous period */

   CtrDrvrREAD_TELEGRAM,          /* Read telegrams from CTR */
   CtrDrvrREAD_EVENT_HISTORY,     /* Read incomming event history */

   /* ============================================================ */
   /* Hardware specialists IOCTL Commands to maintain and diagnose */
   /* the chips on the CTR board. Not for normal timing users.     */

   CtrDrvrIOCTL_49,               /* Spare */
   CtrDrvrIOCTL_50,               /* Spare */
   CtrDrvrIOCTL_51,               /* Spare */
   CtrDrvrIOCTL_52,               /* Spare */

   CtrDrvrHPTDC_OPEN,             /* Open HPTDC JTAG interface */
   CtrDrvrHPTDC_IO,               /* Perform HPTDC IO operation */
   CtrDrvrHPTDC_CLOSE,            /* Close HPTDC JTAG interface */

   CtrDrvrRAW_READ,               /* Raw read  access to mapped card for debug */
   CtrDrvrRAW_WRITE,              /* Raw write access to mapped card for debug */

   CtrDrvrGET_RECEPTION_ERRORS,   /* Timing fram reception error status */
   CtrDrvrGET_IO_STATUS,          /* Status of module inputs */
   CtrDrvrGET_IDENTITY,           /* Identity of board from ID chip */

   CtrDrvrSET_DEBUG_HISTORY,      /* All events get logged in event history */
   CtrDrvrSET_BRUTAL_PLL,         /* Control how UTC PLL relocks */
   CtrDrvrGET_MODULE_STATS,       /* Get module statistics */

   CtrDrvrSET_CABLE_ID,           /* Needed when no ID events sent */

   CtrDrvrLOCK,                   /* Lock all write access to the current CTR module configuration */
   CtrDrvrUNLOCK,                 /* Unlock all write access to the current CTR module configuration */

   CtrDrvrIOCTL_67,               /* Spare */
   CtrDrvrIOCTL_68,               /* Spare */
   CtrDrvrIOCTL_69,               /* Spare */
   CtrDrvrIOCTL_70,               /* Spare */

   /* ============================================================ */
   /* Module specific IOCTL commands, Can't be used in a library!! */

   CtrDrvrGET_OUTPUT_BYTE,        /* VME P2 output byte number */
   CtrDrvrSET_OUTPUT_BYTE,        /* VME P2 output byte number */

   CtrDrvrSET_MODULE_BY_SLOT,     /* Select the module to work with by slot ID */
   CtrDrvrGET_MODULE_SLOT,        /* Get the slot ID of the selected module */

   CtrDrvrLAST_IOCTL

 } CtrDrvrControlFunction;

/*
 * Set up the IOCTL numbers
 */

#define MAGIC 'C'

#define CIO(nr)      _IO(MAGIC,nr)
#define CIOR(nr,sz)  _IOR(MAGIC,nr,sz)
#define CIOW(nr,sz)  _IOW(MAGIC,nr,sz)
#define CIOWR(nr,sz) _IOWR(MAGIC,nr,sz)

#define CtrIoctlSET_SW_DEBUG                   CIOWR(CtrDrvrSET_SW_DEBUG           ,uint32_t)
#define CtrIoctlGET_SW_DEBUG                   CIOWR(CtrDrvrGET_SW_DEBUG           ,uint32_t)
#define CtrIoctlGET_VERSION                    CIOWR(CtrDrvrGET_VERSION            ,CtrDrvrVersion)
#define CtrIoctlSET_TIMEOUT                    CIOWR(CtrDrvrSET_TIMEOUT            ,uint32_t)
#define CtrIoctlGET_TIMEOUT                    CIOWR(CtrDrvrGET_TIMEOUT            ,uint32_t)
#define CtrIoctlSET_QUEUE_FLAG                 CIOWR(CtrDrvrSET_QUEUE_FLAG         ,uint32_t)
#define CtrIoctlGET_QUEUE_FLAG                 CIOWR(CtrDrvrGET_QUEUE_FLAG         ,uint32_t)
#define CtrIoctlGET_QUEUE_SIZE                 CIOWR(CtrDrvrGET_QUEUE_SIZE         ,uint32_t)
#define CtrIoctlGET_QUEUE_OVERFLOW             CIOWR(CtrDrvrGET_QUEUE_OVERFLOW     ,uint32_t)
#define CtrIoctlGET_MODULE_DESCRIPTOR          CIOWR(CtrDrvrGET_MODULE_DESCRIPTOR  ,CtrDrvrModuleAddress)
#define CtrIoctlSET_MODULE                     CIOWR(CtrDrvrSET_MODULE             ,uint32_t)
#define CtrIoctlGET_MODULE                     CIOWR(CtrDrvrGET_MODULE             ,uint32_t)
#define CtrIoctlGET_MODULE_COUNT               CIOWR(CtrDrvrGET_MODULE_COUNT       ,uint32_t)
#define CtrIoctlRESET                          CIOWR(CtrDrvrRESET                  ,uint32_t)
#define CtrIoctlENABLE                         CIOWR(CtrDrvrENABLE                 ,uint32_t)
#define CtrIoctlGET_STATUS                     CIOWR(CtrDrvrGET_STATUS             ,uint32_t)
#define CtrIoctlGET_INPUT_DELAY                CIOWR(CtrDrvrGET_INPUT_DELAY        ,uint32_t)
#define CtrIoctlSET_INPUT_DELAY                CIOWR(CtrDrvrSET_INPUT_DELAY        ,uint32_t)
#define CtrIoctlGET_CLIENT_LIST                CIOWR(CtrDrvrGET_CLIENT_LIST        ,CtrDrvrClientList)
#define CtrIoctlCONNECT                        CIOWR(CtrDrvrCONNECT                ,CtrDrvrConnection)
#define CtrIoctlDISCONNECT                     CIOWR(CtrDrvrDISCONNECT             ,CtrDrvrConnection)
#define CtrIoctlGET_CLIENT_CONNECTIONS         CIOWR(CtrDrvrGET_CLIENT_CONNECTIONS ,CtrDrvrClientConnections)
#define CtrIoctlSET_UTC                        CIOWR(CtrDrvrSET_UTC                ,CtrDrvrCTime)
#define CtrIoctlGET_UTC                        CIOWR(CtrDrvrGET_UTC                ,CtrDrvrCTime)
#define CtrIoctlGET_CABLE_ID                   CIOWR(CtrDrvrGET_CABLE_ID           ,uint32_t)
#define CtrIoctlGET_ACTION                     CIOWR(CtrDrvrGET_ACTION             ,CtrDrvrAction)
#define CtrIoctlSET_ACTION                     CIOWR(CtrDrvrSET_ACTION             ,CtrDrvrAction)
#define CtrIoctlCREATE_CTIM_OBJECT             CIOWR(CtrDrvrCREATE_CTIM_OBJECT     ,CtrDrvrCtimBinding)
#define CtrIoctlDESTROY_CTIM_OBJECT            CIOWR(CtrDrvrDESTROY_CTIM_OBJECT    ,CtrDrvrCtimBinding)
#define CtrIoctlLIST_CTIM_OBJECTS              CIO  (CtrDrvrLIST_CTIM_OBJECTS)   // CtrDrvrCtimObjects too big
#define CtrIoctlCHANGE_CTIM_FRAME              CIOWR(CtrDrvrCHANGE_CTIM_FRAME      ,CtrDrvrCtimBinding)
#define CtrIoctlCREATE_PTIM_OBJECT             CIOWR(CtrDrvrCREATE_PTIM_OBJECT     ,CtrDrvrPtimBinding)
#define CtrIoctlDESTROY_PTIM_OBJECT            CIOWR(CtrDrvrDESTROY_PTIM_OBJECT    ,CtrDrvrPtimBinding)
#define CtrIoctlLIST_PTIM_OBJECTS              CIO  (CtrDrvrLIST_PTIM_OBJECTS)   // CtrDrvrPtimObjects too big
#define CtrIoctlGET_PTIM_BINDING               CIOWR(CtrDrvrGET_PTIM_BINDING       ,CtrDrvrPtimBinding)
#define CtrIoctlGET_OUT_MASK                   CIOWR(CtrDrvrGET_OUT_MASK           ,CtrDrvrCounterMaskBuf)
#define CtrIoctlSET_OUT_MASK                   CIOWR(CtrDrvrSET_OUT_MASK           ,CtrDrvrCounterMaskBuf)
#define CtrIoctlGET_COUNTER_HISTORY            CIOWR(CtrDrvrGET_COUNTER_HISTORY    ,CtrDrvrCounterHistoryBuf)
#define CtrIoctlGET_REMOTE                     CIOWR(CtrDrvrGET_REMOTE             ,CtrdrvrRemoteCommandBuf)
#define CtrIoctlSET_REMOTE                     CIOWR(CtrDrvrSET_REMOTE             ,CtrdrvrRemoteCommandBuf)
#define CtrIoctlREMOTE                         CIOWR(CtrDrvrREMOTE                 ,CtrdrvrRemoteCommandBuf)
#define CtrIoctlGET_CONFIG                     CIOWR(CtrDrvrGET_CONFIG             ,CtrDrvrCounterConfigurationBuf)
#define CtrIoctlSET_CONFIG                     CIOWR(CtrDrvrSET_CONFIG             ,CtrDrvrCounterConfigurationBuf)
#define CtrIoctlGET_PLL                        CIOWR(CtrDrvrGET_PLL                ,CtrDrvrPll)
#define CtrIoctlSET_PLL                        CIOWR(CtrDrvrSET_PLL                ,CtrDrvrPll)
#define CtrIoctlSET_PLL_ASYNC_PERIOD           CIOWR(CtrDrvrSET_PLL_ASYNC_PERIOD   ,CtrDrvrPllAsyncPeriodNs)
#define CtrIoctlGET_PLL_ASYNC_PERIOD           CIOWR(CtrDrvrGET_PLL_ASYNC_PERIOD   ,CtrDrvrPllAsyncPeriodNs)
#define CtrIoctlREAD_TELEGRAM                  CIOWR(CtrDrvrREAD_TELEGRAM          ,CtrDrvrTgmBuf)
#define CtrIoctlREAD_EVENT_HISTORY             CIOWR(CtrDrvrREAD_EVENT_HISTORY     ,CtrDrvrEventHistoryBuf) // too big
#define CtrIoctlHPTDC_OPEN                     CIOWR(CtrDrvrHPTDC_OPEN             ,uint32_t)
#define CtrIoctlHPTDC_IO                       CIOWR(CtrDrvrHPTDC_IO               ,CtrDrvrHptdcIoBuf)
#define CtrIoctlHPTDC_CLOSE                    CIOWR(CtrDrvrHPTDC_CLOSE            ,uint32_t)
#define CtrIoctlRAW_READ                       CIOWR(CtrDrvrRAW_READ               ,CtrDrvrRawIoBlock)
#define CtrIoctlRAW_WRITE                      CIOWR(CtrDrvrRAW_WRITE              ,CtrDrvrRawIoBlock)
#define CtrIoctlGET_RECEPTION_ERRORS           CIOWR(CtrDrvrGET_RECEPTION_ERRORS   ,CtrDrvrReceptionErrors)
#define CtrIoctlGET_IO_STATUS                  CIOWR(CtrDrvrGET_IO_STATUS          ,uint32_t)
#define CtrIoctlGET_IDENTITY                   CIOWR(CtrDrvrGET_IDENTITY           ,CtrDrvrBoardId)
#define CtrIoctlSET_DEBUG_HISTORY              CIOWR(CtrDrvrSET_DEBUG_HISTORY      ,uint32_t)
#define CtrIoctlSET_BRUTAL_PLL                 CIOWR(CtrDrvrSET_BRUTAL_PLL         ,uint32_t)
#define CtrIoctlGET_MODULE_STATS               CIOWR(CtrDrvrGET_MODULE_STATS       ,CtrDrvrModuleStats)
#define CtrIoctlSET_CABLE_ID                   CIOWR(CtrDrvrSET_CABLE_ID           ,uint32_t)

#define CtrIoctlLOCK                           CIOWR(CtrDrvrLOCK                   ,uint32_t)
#define CtrIoctlUNLOCK                         CIOWR(CtrDrvrUNLOCK                 ,uint32_t)

#define CtrIoctlGET_OUTPUT_BYTE                CIOWR(CtrDrvrGET_OUTPUT_BYTE        ,uint32_t)
#define CtrIoctlSET_OUTPUT_BYTE                CIOWR(CtrDrvrSET_OUTPUT_BYTE        ,uint32_t)

#define CtrIoctlSET_MODULE_BY_SLOT             CIOWR(CtrDrvrSET_MODULE_BY_SLOT     ,uint32_t)
#define CtrIoctlGET_MODULE_SLOT                CIOWR(CtrDrvrGET_MODULE_SLOT        ,uint32_t)
#endif
