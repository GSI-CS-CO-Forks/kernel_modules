static __attribute__((unused)) char *rcsid = "$Id: pickering4to1.c,v 1.4 2005/06/29 07:53:20 sdeghaye Exp $";

#include <asm/uaccess.h>
#include "pickering4to1.h"

#define INPUT1_CMD 0x9
#define INPUT2_CMD 0x1
#define INPUT3_CMD 0x4
#define INPUT4_CMD 0x0
#define PICK_MUX_4_TO_1_MAX_OUTPUT 1
#define PICK_MUX_4_TO_1_MAX_INPUT 4

/***************************************************************************************************
 *
 */
int pickMux4to1Probe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{      
  if(pcidev->subsystem_device != PICK_MUX_4_TO_1_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux4to1Probe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  
  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickMux4to1Probe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringmux4to1"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickMux4to1Probe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickMux4to1Probe: LAS0 remap @ 0x%lx (%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* LAS1 memory address space is region 3 (BAR3) */
  muxDesc->las1addr = pci_resource_start(pcidev, 3);
  muxDesc->las1len = pci_resource_len(pcidev, 3);
  printk(KERN_INFO "pickMux4to1Probe: LAS1 @ 0x%lx, size=%d\n", muxDesc->las1addr
                                                              , muxDesc->las1len);
  if (!request_mem_region(muxDesc->las1addr, muxDesc->las1len, "pickeringmux4to1"))
  {
    iounmap(muxDesc->las0);
    release_mem_region(muxDesc->las0addr, muxDesc->las0len);
    printk(KERN_CRIT "pickMux4to1Probe: Unable to request LAS1 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las1addr
                                                               , muxDesc->las1len);
    return -1;
  }
  muxDesc->las1 = ioremap(muxDesc->las1addr, muxDesc->las1len);
  printk(KERN_INFO "pickMux4to1Probe: LAS1 remap @ 0x%lx (%p), size=%d\n", muxDesc->las1addr
                                                                         , muxDesc->las1
                                                                         , muxDesc->las1len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_MUX_4_TO_1_MAX_OUTPUT, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickMux4to1Probe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_MUX_4_TO_1_MAX_OUTPUT);
  
  /* Init the hardware and the status */
  pickMux4to1Reset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void pickMux4to1Remove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickMux4to1Remove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);
  if(muxDesc->type != PICK_MUX_4_TO_1_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux4to1Remove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return;
  }
  
  /* Release reserved resources */
  iounmap(muxDesc->las0);
  release_mem_region(muxDesc->las0addr, muxDesc->las0len);
  iounmap(muxDesc->las1);
  release_mem_region(muxDesc->las1addr, muxDesc->las1len);
  kfree(muxDesc->inputs);  
}

/***************************************************************************************************
 *
 */
int pickMux4to1MaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_MUX_4_TO_1_MAX_OUTPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux4to1MaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_MUX_4_TO_1_MAX_INPUT;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux4to1Connect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{  
  if(muxDesc->type != PICK_MUX_4_TO_1_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux4to1Connect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  
  switch(data.input)
  {
  case 1:
    *(int *)muxDesc->las1 = INPUT1_CMD;
    printk(KERN_DEBUG "pickMux4to1Connect: write %d @ 0x%p\n",INPUT1_CMD, muxDesc->las1);
    break;
  case 2:
    *(int *)muxDesc->las1 = INPUT2_CMD;
    printk(KERN_DEBUG "pickMux4to1Connect: write %d @ 0x%p\n", INPUT2_CMD, muxDesc->las1);
    break;
  case 3:
    *(int *)muxDesc->las1 = INPUT3_CMD;
    printk(KERN_DEBUG "pickMux4to1Connect: write %d @ 0x%p\n", INPUT3_CMD, muxDesc->las1);
    break;
  case 4:
    *(int *)muxDesc->las1 = INPUT4_CMD;
    printk(KERN_DEBUG "pickMux4to1Connect: write %d @ 0x%p\n", INPUT4_CMD, muxDesc->las1);
    break;
  default:
    printk(KERN_ERR "pickMux4to1Connect: write: Unknown input %d\n", data.input);
    return -1;
  }
  muxDesc->inputs[0] = data.input;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux4to1Disconnect(PickeringMuxDescr *muxDesc, int output)
{
  if(muxDesc->type != PICK_MUX_4_TO_1_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickMux4to1Disconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  
  *(int *)muxDesc->las1 = INPUT4_CMD;
  printk(KERN_DEBUG "pickMux4to1Disconnect: write %d @ 0x%p\n", INPUT4_CMD, muxDesc->las1);
  muxDesc->inputs[0] = 4;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickMux4to1Reset(PickeringMuxDescr *muxDesc)
{
  return pickMux4to1Disconnect(muxDesc, 1);
}

/***************************************************************************************************
 *
 */
int pickMux4to1Status(PickeringMuxDescr *muxDesc, int *status)
{
  int rc;
  int size;
  
  size = sizeof(int)*PICK_MUX_4_TO_1_MAX_OUTPUT;
  rc = copy_to_user(status, muxDesc->inputs, size);
  if(rc != 0)
  {
    printk(KERN_ERR "pickMux4to1Status: copy not done correctly from %p to %p, size = %d, rc = %d.\n"
                                                                          , muxDesc->inputs
                                                                          , status
                                                                          , size
                                                                          , rc);
    return rc;
  }
  return rc; 
}
