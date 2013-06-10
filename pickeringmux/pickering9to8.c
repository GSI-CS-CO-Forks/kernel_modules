static char *rcsid = "$Id: pickering9to8.c,v 1.5 2012/04/18 13:01:32 aradeva Exp $";

#include <asm/uaccess.h>
#include "pickering9to8.h"

#define PICK_MUX_9_TO_8_MAX_OUTPUT 8
#define PICK_MUX_9_TO_8_MAX_INPUT 9
#define PICK_MUX_9_TO_8_LOOP_LEN 56
#define PICK_MUX_9_TO_8_NBR_LOOP 2

/* Static global variables */
static RelayMap relayMap[PICK_MUX_9_TO_8_MAX_INPUT][PICK_MUX_9_TO_8_MAX_OUTPUT] = 
{
  {{107,111, 88}, {107,110, 89}, {107,109, 90}, {107,108, 91}, { 12, 16, 31}, { 12, 17, 28}, { 12, 18, 29}, { 12, 16, 30}},
  {{106,101, 88}, {106,100, 89}, {106,104, 90}, {106,105, 91}, { 13, 10, 31}, { 13,  9, 28}, { 13,  8, 29}, { 13, 14, 30}},
  {{102, 98, 88}, {102, 97, 89}, {102, 99, 90}, {102,103, 91}, { 21, 22, 31}, { 21, 23, 28}, { 21, 15, 29}, { 21, 11, 30}},
  {{ 96, 94, 88}, { 96, 95, 89}, { 96, 93, 90}, { 96, 92, 91}, { 20, 24, 31}, { 20, 27, 28}, { 20, 26, 29}, { 20, 25, 30}},
  {{ 83, 80, 85}, { 83, 76, 84}, { 83, 81, 86}, { 83, 82, 87}, { 39, 38, 32}, { 39, 37, 33}, { 39, 36, 34}, { 39, 40, 35}},
  {{ 77, 79, 85}, { 77, 75, 84}, { 77, 74, 86}, { 77, 78, 87}, { 41, 42, 32}, { 41, 43, 33}, { 41, 47, 34}, { 41, 46, 35}},
  {{ 73, 68, 85}, { 73, 69, 84}, { 73, 70, 86}, { 73, 72, 87}, { 45, 44, 32}, { 45, 48, 33}, { 45, 49, 34}, { 45, 50, 35}},
  {{ 64, 67, 85}, { 64, 66, 84}, { 64, 71, 86}, { 64, 65, 87}, { 51,  3, 32}, { 51,  7, 33}, { 51,  2, 34}, { 51,  6, 35}},
  {{ 59, 61, 85}, { 59, 60, 84}, { 59, 62, 86}, { 59, 63, 87}, {  5,  0, 32}, {  5,  4, 33}, {  5, 55, 34}, {  5, 54, 35}}          
};
    
/***************************************************************************************************
 *
 */
int pickMux9to8Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{      
  if(pcidev->subsystem_device != PICK_MUX_9_TO_8_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux9to8Probe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  
  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickMux9to8Probe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringmux9to8"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickMux9to8Probe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickMux9to8Probe: LAS0 remap @ 0x%lx (%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_MUX_9_TO_8_MAX_OUTPUT, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickMux9to8Probe: Unable to allocate memory for the inputs buffer\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_9_TO_8_MAX_OUTPUT);
  
  /* Allocate space to store loop data */
  muxDesc->loopData = (int *)kmalloc(sizeof(int) * PICK_MUX_9_TO_8_LOOP_LEN * PICK_MUX_9_TO_8_NBR_LOOP, GFP_KERNEL);
  if(muxDesc->loopData == NULL)
  {
    printk(KERN_CRIT "pickMux9to8Probe: Unable to allocate memory for the loop data buffer\n");
    return -1;
  }
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_9_TO_8_LOOP_LEN * PICK_MUX_9_TO_8_NBR_LOOP);
  
  /* Init the hardware and the status */
  pickMux9to8Reset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void pickMux9to8Remove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickMux9to8Remove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);

  if(muxDesc->type != PICK_MUX_9_TO_8_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux9to8Remove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
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
int pickMux9to8MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_MUX_9_TO_8_MAX_OUTPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux9to8MaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_MUX_9_TO_8_MAX_INPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux9to8Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{
  if(muxDesc->type != PICK_MUX_9_TO_8_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux9to8Connect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux9to8Connect: trying to connect %d on %d \n", data.input, data.output);
  
  /* Check the output is free */
  if(muxDesc->inputs[data.output - 1] != 0)
  {
    printk(KERN_NOTICE "pickMux9to8Connect: Output %d already used\n", data.output);
    return -1;
  }
  
  /* Set into the loops data the relays involved in the connection */
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yIsoRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yxRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].xIsoRelay] = 1;
  
  /* Register the new connection */
  muxDesc->inputs[data.output - 1] = data.input;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_9_TO_8_NBR_LOOP, PICK_MUX_9_TO_8_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux9to8Disconnect(PickeringMuxDescr *muxDesc, int output)
{
  int i;
  int firstOutput;
  int lastOutput;
  int input;
  int multiConnection;
  
  if(muxDesc->type != PICK_MUX_9_TO_8_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux9to8Disconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux9to8Disconnect");
  
  /* Check the output is connected */
  input = muxDesc->inputs[output - 1];
  if(input == 0)
  {
    printk(KERN_NOTICE "pickMux9to8Disconnect: Output %d already free\n", output);
    return 0;
  }
  
  /* Register the disconnection */
  muxDesc->inputs[output - 1] = 0;
  
  /* Check the input is not connected to another output */
  /* An input is connected to the first half of the outputs through a matrix 
     and to the last half of the outputs through another one */
  if(output <=  (PICK_MUX_9_TO_8_MAX_INPUT) /2 ) 
  {
    /* first matrix => a set of relay */
    firstOutput = 0;
    lastOutput = 3;
  }
  else
  {
    /* second matrix => another set of relay */
    firstOutput = 4;
    lastOutput = 7;
  }
  multiConnection = 0;
  for(i = firstOutput; i <= lastOutput; i++)
  {
    if(muxDesc->inputs[i] == input)
    {
      multiConnection = 1;
      printk(KERN_DEBUG "pickMux9to8Disconnect: Multiconnection...");
      break;
    }
  }
  
  /* Reset into the loops data the relays involved in the connection */
  if(multiConnection == 0)
  {
    muxDesc->loopData[relayMap[input - 1][output - 1].yIsoRelay] = 0;
  }
  muxDesc->loopData[relayMap[input - 1][output - 1].yxRelay] = 0;
  muxDesc->loopData[relayMap[input - 1][output - 1].xIsoRelay] = 0;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_9_TO_8_NBR_LOOP, PICK_MUX_9_TO_8_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux9to8Reset(PickeringMuxDescr *muxDesc)
{
  if(muxDesc->type != PICK_MUX_9_TO_8_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux9to8Reset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  /* no output connected */
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_9_TO_8_MAX_OUTPUT);
  /* no relay closed */
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_9_TO_8_LOOP_LEN * PICK_MUX_9_TO_8_NBR_LOOP);
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_9_TO_8_NBR_LOOP, PICK_MUX_9_TO_8_LOOP_LEN);
    
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux9to8Status(PickeringMuxDescr *muxDesc, int *status)
{
  int rc;
  int size;
  
  size = sizeof(int) * PICK_MUX_9_TO_8_MAX_OUTPUT;
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
