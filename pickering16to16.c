static char *rcsid = "$Id: pickering16to16.c,v 1.6 2012/04/18 13:01:32 aradeva Exp $";

#include <asm/uaccess.h>
#include "pickering16to16.h"

#define PICK_MUX_16_TO_16_MAX_OUTPUT 16
#define PICK_MUX_16_TO_16_MAX_INPUT 16
#define PICK_MUX_16_TO_16_LOOP_LEN 336
#define PICK_MUX_16_TO_16_NBR_LOOP 1

/* Static global variables */
static RelayMap relayMap[PICK_MUX_16_TO_16_MAX_INPUT][PICK_MUX_16_TO_16_MAX_OUTPUT] = 
    {
      {{231,168,164}, {231,187,165}, {231,180,185}, {231,203,183}, {231,192,207}, {231,223,193}, {231,208,221}, {231,235,211}, {248,244,268}, {248,271,258}, {248,256,286}, {248,287,272}, {248,276,296}, {248,299,294}, {248,292,314}, {248,311,315}},
      {{230,169,164}, {230,186,165}, {230,181,185}, {230,202,183}, {230,196,207}, {230,222,193}, {230,209,221}, {230,234,211}, {249,245,268}, {249,270,258}, {249,257,286}, {249,283,272}, {249,277,296}, {249,298,294}, {249,293,314}, {249,310,315}},
      {{229,170,164}, {229,163,165}, {229,182,185}, {229,201,183}, {229,197,207}, {229,219,193}, {229,210,221}, {229,233,211}, {250,246,268}, {250,269,258}, {250,260,286}, {250,282,272}, {250,278,296}, {250,297,294}, {250,316,314}, {250,309,315}},
      {{228,171,164}, {228,162,165}, {228,188,185}, {228,200,183}, {228,198,207}, {228,218,193}, {228,212,221}, {228,232,211}, {251,247,268}, {251,267,258}, {251,261,286}, {251,281,272}, {251,279,296}, {251,291,294}, {251,317,314}, {251,308,315}},
      {{227,172,164}, {227,161,165}, {227,189,185}, {227,179,183}, {227,199,207}, {227,217,193}, {227,213,221}, {227,239,211}, {252,240,268}, {252,266,258}, {252,262,286}, {252,280,272}, {252,300,296}, {252,290,294}, {252,318,314}, {252,307,315}},
      {{226,173,164}, {226,160,165}, {226,190,185}, {226,178,183}, {226,204,207}, {226,216,193}, {226,214,221}, {226,238,211}, {253,241,268}, {253,265,258}, {253,263,286}, {253,275,272}, {253,301,296}, {253,289,294}, {253,319,314}, {253,306,315}},
      {{225,174,164}, {225,167,165}, {225,191,185}, {225,177,183}, {225,205,207}, {225,195,193}, {225,215,221}, {225,237,211}, {254,242,268}, {254,264,258}, {254,284,286}, {254,274,272}, {254,302,296}, {254,288,294}, {254,312,314}, {254,305,315}},
      {{224,175,164}, {224,166,165}, {224,184,185}, {224,176,183}, {224,206,207}, {224,194,193}, {224,220,221}, {224,236,211}, {255,243,268}, {255,259,258}, {255,285,286}, {255,273,272}, {255,303,296}, {255,295,294}, {255,313,314}, {255,304,315}},
      {{ 95,144,155}, { 95,153,154}, { 95,135,134}, { 95,143,136}, { 95,113,112}, { 95,125,126}, { 95, 99, 98}, { 95, 83,108}, { 64, 76, 51}, { 64, 60, 61}, { 64, 34, 33}, { 64, 46, 47}, { 64, 16, 23}, { 64, 24, 25}, { 64,  6,  5}, { 64, 15,  4}},
      {{ 94,145,155}, { 94,152,154}, { 94,128,134}, { 94,142,136}, { 94,114,112}, { 94,124,126}, { 94,104, 98}, { 94, 82,108}, { 65, 77, 51}, { 65, 55, 61}, { 65, 35, 33}, { 65, 45, 47}, { 65, 17, 23}, { 65, 31, 25}, { 65,  7,  5}, { 65, 14,  4}},
      {{ 93,146,155}, { 93,159,154}, { 93,129,134}, { 93,141,136}, { 93,115,112}, { 93,103,126}, { 93,105, 98}, { 93, 81,108}, { 66, 78, 51}, { 66, 54, 61}, { 66, 56, 33}, { 66, 44, 47}, { 66, 18, 23}, { 66, 30, 25}, { 66,  0,  5}, { 66, 13,  4}},
      {{ 92,147,155}, { 92,158,154}, { 92,130,134}, { 92,140,136}, { 92,120,112}, { 92,102,126}, { 92,106, 98}, { 92, 80,108}, { 67, 79, 51}, { 67, 53, 61}, { 67, 57, 33}, { 67, 39, 47}, { 67, 19, 23}, { 67, 29, 25}, { 67,  1,  5}, { 67, 12,  4}},
      {{ 91,148,155}, { 91,157,154}, { 91,131,134}, { 91,119,136}, { 91,121,112}, { 91,101,126}, { 91,107, 98}, { 91, 87,108}, { 68, 72, 51}, { 68, 52, 61}, { 68, 58, 33}, { 68, 38, 47}, { 68, 40, 23}, { 68, 28, 25}, { 68,  2,  5}, { 68, 11,  4}},
      {{ 90,149,155}, { 90,156,154}, { 90,137,134}, { 90,118,136}, { 90,122,112}, { 90,100,126}, { 90,109, 98}, { 90, 86,108}, { 69, 73, 51}, { 69, 50, 61}, { 69, 59, 33}, { 69, 37, 47}, { 69, 41, 23}, { 69, 22, 25}, { 69,  3,  5}, { 69, 10,  4}},
      {{ 89,150,155}, { 89,133,154}, { 89,138,134}, { 89,117,136}, { 89,123,112}, { 89, 97,126}, { 89,110, 98}, { 89, 85,108}, { 70, 74, 51}, { 70, 49, 61}, { 70, 62, 33}, { 70, 36, 47}, { 70, 42, 23}, { 70, 21, 25}, { 70, 26,  5}, { 70,  9,  4}},
      {{ 88,151,155}, { 88,132,154}, { 88,139,134}, { 88,116,136}, { 88,127,112}, { 88, 96,126}, { 88,111, 98}, { 88, 84,108}, { 71, 75, 51}, { 71, 48, 61}, { 71, 63, 33}, { 71, 32, 47}, { 71, 43, 23}, { 71, 20, 25}, { 71, 27,  5}, { 71,  8,  4}}
    };
    
static int loopThruMap[PICK_MUX_16_TO_16_MAX_OUTPUT] = {334, 335, 332, 333, 330, 331, 328, 329, 326, 327, 324, 325, 322, 323, 320, 321};
 
/***************************************************************************************************
 *
 */
int pickMux16to16Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{      
  if(pcidev->subsystem_device != PICK_MUX_16_TO_16_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16Probe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  
  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickMux16to16Probe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringmux16to16"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickMux16to16Probe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickMux16to16Probe: LAS0 remap @ 0x%lx (0x%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_MUX_16_TO_16_MAX_OUTPUT, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickMux16to16Probe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_16_TO_16_MAX_OUTPUT);
  
  /* Allocate space to store loop data */
  muxDesc->loopData = (int *)kmalloc(sizeof(int) * PICK_MUX_16_TO_16_LOOP_LEN * PICK_MUX_16_TO_16_NBR_LOOP, GFP_KERNEL);
  if(muxDesc->loopData == NULL)
  {
    printk(KERN_CRIT "pickMux16to16Probe: Unable to allocate memory for the loop data buffer\n");
    return -1;
  }
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_16_TO_16_LOOP_LEN * PICK_MUX_16_TO_16_NBR_LOOP);
  
  /* Init the hardware and the status */
  pickMux16to16Reset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void pickMux16to16Remove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickMux16to16Remove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);
  
  if(muxDesc->type != PICK_MUX_16_TO_16_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16Remove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
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
int pickMux16to16MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_MUX_16_TO_16_MAX_OUTPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16MaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_MUX_16_TO_16_MAX_INPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{
  if(muxDesc->type != PICK_MUX_16_TO_16_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16Connect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux16to16Connect: trying to connect %d on %d \n", data.input, data.output);
  
  /* Check the output is free */
  if(muxDesc->inputs[data.output - 1] != 0)
  {
    printk(KERN_NOTICE "pickMux16to16Connect: Output %d already used\n", data.output);
    return -1;
  }
  
  /* Set into the loop data the relays involved in the connection */
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yIsoRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].yxRelay] = 1;
  muxDesc->loopData[relayMap[data.input - 1][data.output - 1].xIsoRelay] = 1;
  
  /* Set into the loop data the loopthru relay */
  muxDesc->loopData[loopThruMap[data.output - 1]] = 1;
  
  /* Register the new connection */
  muxDesc->inputs[data.output - 1] = data.input;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16_NBR_LOOP, PICK_MUX_16_TO_16_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16Disconnect(PickeringMuxDescr *muxDesc, int output)
{
  int i;
  int input;
  int firstOutput;
  int lastOutput;
  int multiConnection;
  
  if(muxDesc->type != PICK_MUX_16_TO_16_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16Disconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickMux16to16Disconnect\n");
  
  /* Check the output is connected */
  input = muxDesc->inputs[output - 1];
  if(input == 0)
  {
    printk(KERN_NOTICE "pickMux16to16Disconnect: Output %d already free\n", output);
    return 0;
  }
  
  /* Register the disconnection */
  muxDesc->inputs[output - 1] = 0;
  
  /* Check the input is not connected to another output */
  /* An input is connected to the first half of the outputs through a matrix 
     and to the last half of the outputs through another one */
  if(output <=  (PICK_MUX_16_TO_16_MAX_INPUT) /2 ) 
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
      printk(KERN_DEBUG "pickMux16to16Disconnect: Multiconnection...\n");
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
  
  /* Reset into the loop data the loopthru relay (close the relay) */
  muxDesc->loopData[loopThruMap[output - 1]] = 0;
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16_NBR_LOOP, PICK_MUX_16_TO_16_LOOP_LEN);
  
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16Reset(PickeringMuxDescr *muxDesc)
{
  if(muxDesc->type != PICK_MUX_16_TO_16_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux16to16Reset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  /* no output connected */
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_16_TO_16_MAX_OUTPUT);
  /* no relay closed */
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_MUX_16_TO_16_LOOP_LEN * PICK_MUX_16_TO_16_NBR_LOOP);
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_MUX_16_TO_16_NBR_LOOP, PICK_MUX_16_TO_16_LOOP_LEN);

  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux16to16Status(PickeringMuxDescr *muxDesc, int *status)
{
  int rc;
  int size;
  
  size = sizeof(int) * PICK_MUX_16_TO_16_MAX_OUTPUT;
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
