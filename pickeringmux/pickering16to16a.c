static char *rcsid = "$Id: pickering16to16a.c,v 1.5 2012/10/08 15:49:46 sdeghaye Exp $";

#include <asm/uaccess.h>
#include "pickering16to16a.h"

#define PICK_MUX_16_TO_16A_MAX_OUTPUT 16
#define PICK_MUX_16_TO_16A_MAX_INPUT 16
#define PICK_MUX_16_TO_16A_LOOP_LEN 192
#define PICK_MUX_16_TO_16A_NBR_LOOP 2

/* Static global variables */
static RelayMap relayMap[PICK_MUX_16_TO_16A_MAX_INPUT][PICK_MUX_16_TO_16A_MAX_OUTPUT] = 
    {
      {{187,191, 63}, {187,190, 62}, {187,188, 60}, {187,189, 61}, {187,176, 48}, {187,177, 49}, {187,178, 50}, {187,179, 51}, {186,185, 59}, {186,184, 58}, {186,180, 57}, {186,181, 56}, {186,182, 52}, {186,183, 53}, {186,247, 54}, {186,255, 55}},
      {{171,175, 63}, {171,174, 62}, {171,172, 60}, {171,173, 61}, {171,160, 48}, {171,161, 49}, {171,162, 50}, {171,163, 51}, {170,169, 59}, {170,168, 58}, {170,164, 57}, {170,165, 56}, {170,166, 52}, {170,167, 53}, {170,246, 54}, {170,254, 55}},
      {{155,159, 63}, {155,158, 62}, {155,156, 60}, {155,157, 61}, {155,144, 48}, {155,145, 49}, {155,146, 50}, {155,147, 51}, {154,153, 59}, {154,152, 58}, {154,148, 57}, {154,149, 56}, {154,150, 52}, {154,151, 53}, {154,245, 54}, {154,253, 55}},
      {{139,143, 63}, {139,142, 62}, {139,140, 60}, {139,141, 61}, {139,128, 48}, {139,129, 49}, {139,130, 50}, {139,131, 51}, {138,137, 59}, {138,136, 58}, {138,132, 57}, {138,133, 56}, {138,134, 52}, {138,135, 53}, {138,244, 54}, {138,252, 55}},
      {{123,127, 47}, {123,126, 46}, {123,124, 44}, {123,125, 45}, {123,112, 32}, {123,113, 33}, {123,114, 34}, {123,115, 35}, {122,121, 43}, {122,120, 42}, {122,116, 41}, {122,117, 40}, {122,118, 36}, {122,119, 37}, {122,243, 38}, {122,251, 39}},
      {{107,111, 47}, {107,110, 46}, {107,108, 44}, {107,109, 45}, {107, 96, 32}, {107, 97, 33}, {107, 98, 34}, {107, 99, 35}, {106,105, 43}, {106,104, 42}, {106,100, 41}, {106,101, 40}, {106,102, 36}, {106,103, 37}, {106,242, 38}, {106,250, 39}},
      {{ 91, 95, 47}, { 91, 94, 46}, { 91, 92, 44}, { 91, 93, 45}, { 91, 80, 32}, { 91, 81, 33}, { 91, 82, 34}, { 91, 83, 35}, { 90, 89, 43}, { 90, 88, 42}, { 90, 84, 41}, { 90, 85, 40}, { 90, 86, 36}, { 90, 87, 37}, { 90,241, 38}, { 90,249, 39}},
      {{ 75, 79, 47}, { 75, 78, 46}, { 75, 76, 44}, { 75, 77, 45}, { 75, 64, 32}, { 75, 65, 33}, { 75, 66, 34}, { 75, 67, 35}, { 74, 73, 43}, { 74, 72, 42}, { 74, 68, 41}, { 74, 69, 40}, { 74, 70, 36}, { 74, 71, 37}, { 74,240, 38}, { 74,248, 39}},
      {{379,383, 31}, {379,382, 30}, {379,380, 28}, {379,381, 29}, {379,368, 16}, {379,369, 17}, {379,370, 18}, {379,371, 19}, {378,377, 27}, {378,376, 26}, {378,372, 25}, {378,373, 24}, {378,374, 20}, {378,375, 21}, {378,231, 22}, {378,239, 23}},
      {{363,367, 31}, {363,366, 30}, {363,364, 28}, {363,365, 29}, {363,352, 16}, {363,353, 17}, {363,354, 18}, {363,355, 19}, {362,361, 27}, {362,360, 26}, {362,356, 25}, {362,357, 24}, {362,358, 20}, {362,359, 21}, {362,230, 22}, {362,238, 23}},
      {{347,351, 31}, {347,350, 30}, {347,348, 28}, {347,349, 29}, {347,336, 16}, {347,337, 17}, {347,338, 18}, {347,339, 19}, {346,345, 27}, {346,344, 26}, {346,340, 25}, {346,341, 24}, {346,342, 20}, {346,343, 21}, {346,229, 22}, {346,237, 23}},
      {{331,335, 31}, {331,334, 30}, {331,332, 28}, {331,331, 29}, {331,320, 16}, {331,321, 17}, {331,322, 18}, {331,323, 19}, {330,329, 27}, {330,328, 26}, {330,324, 25}, {330,325, 24}, {330,326, 20}, {330,327, 21}, {330,228, 22}, {330,236, 23}},
      {{315,319, 15}, {315,318, 14}, {315,316, 12}, {315,317, 13}, {315,304,  0}, {315,305,  1}, {315,306,  2}, {315,307,  3}, {314,313, 11}, {314,312, 10}, {314,308,  9}, {314,309,  8}, {314,310,  4}, {314,311,  5}, {314,227,  6}, {314,235,  7}},
      {{299,303, 15}, {299,302, 14}, {299,300, 12}, {299,301, 13}, {299,288,  0}, {299,289,  1}, {299,290,  2}, {299,291,  3}, {298,297, 11}, {298,296, 10}, {298,292,  9}, {298,293,  8}, {298,294,  4}, {298,295,  5}, {298,226,  6}, {298,234,  7}},
      {{283,287, 15}, {283,286, 14}, {283,284, 12}, {283,285, 13}, {283,272,  0}, {283,273,  1}, {283,274,  2}, {283,275,  3}, {282,281, 11}, {282,280, 10}, {282,276,  9}, {282,277,  8}, {282,278,  4}, {282,279,  5}, {282,225,  6}, {282,233,  7}},
      {{267,271, 15}, {267,270, 14}, {267,268, 12}, {267,269, 13}, {267,256,  0}, {267,257,  1}, {267,258,  2}, {267,259,  3}, {266,265, 11}, {266,264, 10}, {266,260,  9}, {266,261,  8}, {266,262,  4}, {266,263,  5}, {266,224,  6}, {266,232,  7}}
    };
    
static int loopThruMap[PICK_MUX_16_TO_16A_MAX_OUTPUT] =  {209, 211, 213, 215, 217, 219, 221, 223, 193, 195, 197, 199, 201, 203, 205, 207};
static int globalIsoMap[PICK_MUX_16_TO_16A_MAX_OUTPUT] = {208, 210, 212, 214, 216, 218, 220, 222, 192, 194, 196, 198, 200, 202, 204, 206};
 
/***************************************************************************************************
 *
 */
int pickMux16to16aProbe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{      
  if(pcidev->subsystem_device != PICK_MUX_16_TO_16A_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16aProbe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  
  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickMux16to16aProbe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringmux16to16a"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickMux16to16aProbe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickMux16to16aProbe: LAS0 remap @ 0x%lx (0x%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_MUX_16_TO_16A_MAX_OUTPUT, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickMux16to16aProbe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_16_TO_16A_MAX_OUTPUT);
  
  /* Allocate space to store loop data */
  muxDesc->loopData = (int *)kmalloc(sizeof(int) * PICK_MUX_16_TO_16A_LOOP_LEN * PICK_MUX_16_TO_16A_NBR_LOOP, GFP_KERNEL);
  if(muxDesc->loopData == NULL)
  {
    printk(KERN_CRIT "pickMux16to16aProbe: Unable to allocate memory for the loop data buffer\n");
    return -1;
  }
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_16_TO_16A_LOOP_LEN * PICK_MUX_16_TO_16A_NBR_LOOP);
  
  /* Init the hardware and the status */
  pickMux16to16aReset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void pickMux16to16aRemove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickMux16to16aRemove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);
  
  if(muxDesc->type != PICK_MUX_16_TO_16A_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16aRemove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
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
int pickMux16to16aMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_MUX_16_TO_16A_MAX_OUTPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16aMaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_MUX_16_TO_16A_MAX_INPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16aConnect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{
  if(muxDesc->type != PICK_MUX_16_TO_16A_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16aConnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux16to16aConnect: trying to connect %d on %d \n", data.input, data.output);
  
  /* Check the output is free */
  if(muxDesc->inputs[data.output - 1] != 0)
  {
    printk(KERN_NOTICE "pickMux16to16aConnect: Output %d already used\n", data.output);
    return -1;
  }
  
  /* Set into the loop data the relays involved in the connection */
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yIsoRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yxRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].xIsoRelay] = 1;
  
  /* Reset into the loop data the loopthru relay and set the global isolation relay*/
  muxDesc->loopData[loopThruMap[data.output - 1]] = 0;
  muxDesc->loopData[globalIsoMap[data.output - 1]] = 1;
  
  /* Register the new connection */
  muxDesc->inputs[data.output - 1] = data.input;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16A_NBR_LOOP, PICK_MUX_16_TO_16A_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16aDisconnect(PickeringMuxDescr *muxDesc, int output)
{
  int i;
  int input;
  int firstOutput;
  int lastOutput;
  int multiConnection;
  
  if(muxDesc->type != PICK_MUX_16_TO_16A_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16aDisconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux16to16aDisconnect\n");
  
  /* Check the output is connected */
  input = muxDesc->inputs[output - 1];
  if(input == 0)
  {
    printk(KERN_NOTICE "pickMux16to16aDisconnect: Output %d already free\n", output);
    return 0;
  }
  
  /* Register the disconnection */
  muxDesc->inputs[output - 1] = 0;
  
  /* Check the input is not connected to another output */
  /* An input is connected to the first half of the outputs through a matrix 
     and to the last half of the outputs through another one */
  if(output <=  (PICK_MUX_16_TO_16A_MAX_INPUT) /2 ) 
  {
    /* first matrix => a set of relay */
    firstOutput = 0;
    lastOutput = 7;
  }
  else
  {
    /* second matrix => another set of relay */
    firstOutput = 8;
    lastOutput = 15;
  }
  multiConnection = 0;
  for(i = firstOutput; i <= lastOutput; i++)
  {
    if(muxDesc->inputs[i] == input)
    {
      multiConnection = 1;
      printk(KERN_DEBUG "pickMux16to16aDisconnect: Multiconnection...\n");
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
  
  /* Reset into the loop data the loopthru relay (close the relay) and set the global isolation relay */
  muxDesc->loopData[loopThruMap[output - 1]] = 1;
  muxDesc->loopData[globalIsoMap[output - 1]] = 0;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16A_NBR_LOOP, PICK_MUX_16_TO_16A_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16aReset(PickeringMuxDescr *muxDesc)
{
  int i;
  
  if(muxDesc->type != PICK_MUX_16_TO_16A_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16aReset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  /* no output connected */
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_16_TO_16A_MAX_OUTPUT);
  /* no relay closed */
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_16_TO_16A_LOOP_LEN * PICK_MUX_16_TO_16A_NBR_LOOP);

  /* In this version, the loop-through relays have an active high logic 
      => Set their bit to close them at reset */
  for(i = 0; i < PICK_MUX_16_TO_16A_MAX_OUTPUT; i++)
  {
    muxDesc->loopData[loopThruMap[i]] = 1;
  }
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16A_NBR_LOOP, PICK_MUX_16_TO_16A_LOOP_LEN);

  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16aStatus(PickeringMuxDescr *muxDesc, int *status)
{
  int rc;
  int size;
  
  size = sizeof(int) * PICK_MUX_16_TO_16A_MAX_OUTPUT;
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
