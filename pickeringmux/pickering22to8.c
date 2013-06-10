static char *rcsid = "$Id: pickering22to8.c,v 1.2 2012/04/18 13:14:00 sdeghaye Exp $";

#include <asm/uaccess.h>
#include "pickering22to8.h"

#define PICK_MUX_22_TO_8_MAX_OUTPUT 8
#define PICK_MUX_22_TO_8_MAX_INPUT 22
#define PICK_MUX_22_TO_8_LOOP_LEN 176
#define PICK_MUX_22_TO_8_NBR_LOOP 1

/* Static global variables */
static RelayMap relayMap[PICK_MUX_22_TO_8_MAX_INPUT][PICK_MUX_22_TO_8_MAX_OUTPUT] = 
    {
      {{0,128,0}, {0,154,0}, {0,  4,0}, {0,175,0}, {0, 44,0}, {0, 54,0}, {0, 53,0}, {0, 52,0}},
      {{0,129,0}, {0,155,0}, {0,  5,0}, {0,174,0}, {0, 45,0}, {0, 49,0}, {0, 48,0}, {0, 55,0}},
      {{0,130,0}, {0,151,0}, {0,  6,0}, {0,173,0}, {0, 46,0}, {0, 60,0}, {0, 51,0}, {0, 50,0}},
      {{0,131,0}, {0,150,0}, {0,  7,0}, {0,172,0}, {0, 47,0}, {0, 63,0}, {0, 62,0}, {0, 61,0}},
      {{0, 18,0}, {0,149,0}, {0,  0,0}, {0,168,0}, {0, 40,0}, {0, 58,0}, {0, 57,0}, {0, 56,0}},
      {{0, 19,0}, {0,148,0}, {0,  1,0}, {0,169,0}, {0, 41,0}, {0, 69,0}, {0, 68,0}, {0, 59,0}},
      {{0, 28,0}, {0,144,0}, {0,  2,0}, {0,170,0}, {0,120,0}, {0, 64,0}, {0, 71,0}, {0, 70,0}},
      {{0, 29,0}, {0,145,0}, {0,  3,0}, {0,171,0}, {0,124,0}, {0, 67,0}, {0, 66,0}, {0, 65,0}},
      {{0, 30,0}, {0,146,0}, {0, 12,0}, {0,167,0}, {0,125,0}, {0, 78,0}, {0, 77,0}, {0, 76,0}},
      {{0, 31,0}, {0,147,0}, {0, 13,0}, {0,166,0}, {0, 43,0}, {0, 73,0}, {0, 72,0}, {0, 79,0}},
      {{0, 24,0}, {0,143,0}, {0, 14,0}, {0,165,0}, {0, 42,0}, {0, 84,0}, {0, 75,0}, {0, 74,0}},
      {{0, 25,0}, {0,142,0}, {0, 15,0}, {0,164,0}, {0,126,0}, {0, 87,0}, {0, 86,0}, {0, 85,0}},
      {{0, 26,0}, {0,141,0}, {0,  8,0}, {0,153,0}, {0,127,0}, {0, 82,0}, {0, 81,0}, {0, 80,0}},
      {{0, 27,0}, {0,140,0}, {0,  9,0}, {0,152,0}, {0, 83,0}, {0,123,0}, {0,122,0}, {0,121,0}},
      {{0, 36,0}, {0,132,0}, {0, 10,0}, {0,156,0}, {0,119,0}, {0,118,0}, {0,117,0}, {0,116,0}},
      {{0, 37,0}, {0,133,0}, {0, 11,0}, {0,157,0}, {0,115,0}, {0,114,0}, {0,113,0}, {0,112,0}},
      {{0, 38,0}, {0,134,0}, {0, 20,0}, {0,158,0}, {0,111,0}, {0,110,0}, {0,109,0}, {0,108,0}},
      {{0, 39,0}, {0,135,0}, {0, 21,0}, {0,159,0}, {0,107,0}, {0,106,0}, {0,105,0}, {0,104,0}},
      {{0, 32,0}, {0,139,0}, {0, 22,0}, {0,163,0}, {0,103,0}, {0,102,0}, {0,101,0}, {0,100,0}},
      {{0, 33,0}, {0,138,0}, {0, 23,0}, {0,162,0}, {0, 99,0}, {0, 98,0}, {0, 97,0}, {0, 96,0}},
      {{0, 34,0}, {0,137,0}, {0, 16,0}, {0,161,0}, {0, 95,0}, {0, 94,0}, {0, 93,0}, {0, 92,0}},
      {{0, 35,0}, {0,136,0}, {0, 17,0}, {0,160,0}, {0, 91,0}, {0, 90,0}, {0, 89,0}, {0, 88,0}}
    };    
 
/***************************************************************************************************
 *
 */
int pickMux22to8Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{      
  if(pcidev->subsystem_device != PICK_MUX_22_TO_8_1_SUBSYSTEM_ID &&
      pcidev->subsystem_device != PICK_MUX_22_TO_8_2_SUBSYSTEM_ID &&
      pcidev->subsystem_device != PICK_MUX_22_TO_8_3_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux22to8Probe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  
  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickMux22to8Probe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringmux22to8"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickMux22to8Probe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickMux22to8Probe: LAS0 remap @ 0x%lx (0x%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_MUX_22_TO_8_MAX_OUTPUT, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickMux22to8Probe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_22_TO_8_MAX_OUTPUT);
  
  /* Allocate space to store loop data */
  muxDesc->loopData = (int *)kmalloc(sizeof(int) * PICK_MUX_22_TO_8_LOOP_LEN * PICK_MUX_22_TO_8_NBR_LOOP, GFP_KERNEL);
  if(muxDesc->loopData == NULL)
  {
    printk(KERN_CRIT "pickMux22to8Probe: Unable to allocate memory for the loop data buffer\n");
    return -1;
  }
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_22_TO_8_LOOP_LEN * PICK_MUX_22_TO_8_NBR_LOOP);
  
  /* Init the hardware and the status */
  pickMux22to8Reset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void pickMux22to8Remove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickMux22to8Remove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);
  
  if(muxDesc->type != PICK_MUX_22_TO_8_1_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_2_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_3_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux22to8Remove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return;
  }

  /* Release reserved resources */
  iounmap(muxDesc->las0);
  release_mem_region(muxDesc->las0addr, muxDesc->las0len);
  kfree(muxDesc->inputs);  
  kfree(muxDesc->loopData);
}

/***************************************************************************************************
 *
 */
int pickMux22to8MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_MUX_22_TO_8_MAX_OUTPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux22to8MaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_MUX_22_TO_8_MAX_INPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux22to8Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{
  if(muxDesc->type != PICK_MUX_22_TO_8_1_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_2_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_3_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux22to8Connect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux22to8Connect: trying to connect %d on %d \n", data.input, data.output);
  
  /* Check the output is free */
  if(muxDesc->inputs[data.output - 1] != 0)
  {
    printk(KERN_NOTICE "pickMux22to8Connect: Output %d already used\n", data.output);
    return -1;
  }
  
  /* Set into the loop data the relays involved in the connection */
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yxRelay] = 1;
  
  /* Register the new connection */
  muxDesc->inputs[data.output - 1] = data.input;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_22_TO_8_NBR_LOOP, PICK_MUX_22_TO_8_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux22to8Disconnect(PickeringMuxDescr *muxDesc, int output)
{
  int input;
  
  if(muxDesc->type != PICK_MUX_22_TO_8_1_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_2_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_3_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux22to8Disconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux22to8Disconnect\n");
  
  /* Check the output is connected */
  input = muxDesc->inputs[output - 1];
  if(input == 0)
  {
    printk(KERN_NOTICE "pickMux22to8Disconnect: Output %d already free\n", output);
    return 0;
  }
  
  /* Register the disconnection */
  muxDesc->inputs[output - 1] = 0;
   
  /* Reset into the loops data the relays involved in the connection */
  muxDesc->loopData[relayMap[input - 1][output - 1].yxRelay] = 0;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_22_TO_8_NBR_LOOP, PICK_MUX_22_TO_8_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux22to8Reset(PickeringMuxDescr *muxDesc)
{
  if(muxDesc->type != PICK_MUX_22_TO_8_1_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_2_SUBSYSTEM_ID &&
      muxDesc->type != PICK_MUX_22_TO_8_3_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux22to8Reset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  /* no output connected */
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_22_TO_8_MAX_OUTPUT);
  /* no relay closed */
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_22_TO_8_LOOP_LEN * PICK_MUX_22_TO_8_NBR_LOOP);
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_22_TO_8_NBR_LOOP, PICK_MUX_22_TO_8_LOOP_LEN);

  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux22to8Status(PickeringMuxDescr *muxDesc, int *status)
{
  int rc;
  int size;
  
  size = sizeof(int) * PICK_MUX_22_TO_8_MAX_OUTPUT;
  rc = copy_to_user(status, muxDesc->inputs, size);
  if(rc != 0)
  {
    printk(KERN_ERR "pickMux9to8Status: copy not done correctly from %p to %p, size = %d, rc = %d.\n"
                                                                          , muxDesc->inputs
                                                                          , status
                                                                          , size
                                                                          , rc);
    return rc;
  }
  return rc;
}
