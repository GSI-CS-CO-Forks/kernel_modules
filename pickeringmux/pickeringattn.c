static __attribute__((unused)) char *rcsid = "$Id: pickeringattn.c,v 1.5 2008/12/04 12:16:18 sdeghaye Exp $";

#include <asm/uaccess.h>
#include "pickeringattn.h"

#define PICK_ATTN_MAX_CHANNEL 2
#define PICK_ATTN_MAX_ATTN 63
#define PICK_ATTN_LOOP_LEN 32
#define PICK_ATTN_NBR_LOOP 1
#define PICK_ATTN_NBR_VALUES 64
#define PICK_ATTN_NBR_ATTN 7
#define PICK_ATTN_IDX_ATTN_1 23
#define PICK_ATTN_IDX_ATTN_2 16

/***************************************************************************************************
 *  This array contains the position where the control word has to be copied in the loop data
 */
static unsigned int ctrlWordPosition[PICK_ATTN_MAX_CHANNEL] = 
{
    PICK_ATTN_IDX_ATTN_1,
    PICK_ATTN_IDX_ATTN_2
};

/***************************************************************************************************
 *  This 2D array contains the control words for all the attenuation values
 */
static unsigned int ctrlWords[PICK_ATTN_NBR_VALUES][PICK_ATTN_NBR_ATTN] =
{
{0, 0, 0, 0, 0, 0, 0},		/*	0dB	*/
{0, 0, 0, 0, 0, 1, 0},		/*	1dB	*/
{0, 0, 0, 0, 1, 0, 0},		/*	2dB	*/
{0, 0, 0, 0, 1, 1, 0},		/*	3dB	*/
{0, 0, 0, 1, 0, 0, 0},		/*	4dB	*/
{0, 0, 0, 1, 0, 1, 0},		/*	5dB	*/
{0, 0, 0, 1, 1, 0, 0},		/*	6dB	*/
{0, 0, 0, 1, 1, 1, 0},		/*	7dB	*/
{0, 0, 1, 0, 0, 0, 0},		/*	8dB	*/
{0, 0, 1, 0, 0, 1, 0},		/*	9dB	*/
{0, 0, 1, 0, 1, 0, 0},		/*	10dB	*/
{0, 0, 1, 0, 1, 1, 0},		/*	11dB	*/
{0, 0, 1, 1, 0, 0, 0},		/*	12dB	*/
{0, 0, 1, 1, 0, 1, 0},		/*	13dB	*/
{0, 0, 1, 1, 1, 0, 0},		/*	14dB	*/
{0, 0, 1, 1, 1, 1, 0},		/*	15dB	*/
{0, 0, 0, 0, 0, 0, 1},		/*	16dB	*/
{0, 0, 0, 0, 0, 1, 1},		/*	17dB	*/
{0, 0, 0, 0, 1, 0, 1},		/*	18dB	*/
{0, 0, 0, 0, 1, 1, 1},		/*	19dB	*/
{0, 0, 0, 1, 0, 0, 1},		/*	20dB	*/
{0, 0, 0, 1, 0, 1, 1},		/*	21dB	*/
{0, 0, 0, 1, 1, 0, 1},		/*	22dB	*/
{0, 0, 0, 1, 1, 1, 1},		/*	23dB	*/
{0, 0, 1, 0, 0, 0, 1},		/*	24dB	*/
{0, 0, 1, 0, 0, 1, 1},		/*	25dB	*/
{0, 0, 1, 0, 1, 0, 1},		/*	26dB	*/
{0, 0, 1, 0, 1, 1, 1},		/*	27dB	*/
{0, 0, 1, 1, 0, 0, 1},		/*	28dB	*/
{0, 0, 1, 1, 0, 1, 1},		/*	29dB	*/
{0, 0, 1, 1, 1, 0, 1},		/*	30dB	*/
{0, 0, 1, 1, 1, 1, 1},		/*	31dB	*/
{0, 1, 0, 0, 0, 0, 1},		/*	32dB	*/
{0, 1, 0, 0, 0, 1, 1},		/*	33dB	*/
{0, 1, 0, 0, 1, 0, 1},		/*	34dB	*/
{0, 1, 0, 0, 1, 1, 1},		/*	35dB	*/
{0, 1, 0, 1, 0, 0, 1},		/*	36dB	*/
{0, 1, 0, 1, 0, 1, 1},		/*	37dB	*/
{0, 1, 0, 1, 1, 0, 1},		/*	38dB	*/
{0, 1, 0, 1, 1, 1, 1},		/*	39dB	*/
{0, 1, 1, 0, 0, 0, 1},		/*	40dB	*/
{0, 1, 1, 0, 0, 1, 1},		/*	41dB	*/
{0, 1, 1, 0, 1, 0, 1},		/*	42dB	*/
{0, 1, 1, 0, 1, 1, 1},		/*	43dB	*/
{0, 1, 1, 1, 0, 0, 1},		/*	44dB	*/
{0, 1, 1, 1, 0, 1, 1},		/*	45dB	*/
{0, 1, 1, 1, 1, 0, 1},		/*	46dB	*/
{0, 1, 1, 1, 1, 1, 1},		/*	47dB	*/
{1, 1, 0, 0, 0, 0, 1},		/*	48dB	*/
{1, 1, 0, 0, 0, 1, 1},		/*	49dB	*/
{1, 1, 0, 0, 1, 0, 1},		/*	50dB	*/
{1, 1, 0, 0, 1, 1, 1},		/*	51dB	*/
{1, 1, 0, 1, 0, 0, 1},		/*	52dB	*/
{1, 1, 0, 1, 0, 1, 1},		/*	53dB	*/
{1, 1, 0, 1, 1, 0, 1},		/*	54dB	*/
{1, 1, 0, 1, 1, 1, 1},		/*	55dB	*/
{1, 1, 1, 0, 0, 0, 1},		/*	56dB	*/
{1, 1, 1, 0, 0, 1, 1},		/*	57dB	*/
{1, 1, 1, 0, 1, 0, 1},		/*	58dB	*/
{1, 1, 1, 0, 1, 1, 1},		/*	59dB	*/
{1, 1, 1, 1, 0, 0, 1},		/*	60dB	*/
{1, 1, 1, 1, 0, 1, 1},		/*	61dB	*/
{1, 1, 1, 1, 1, 0, 1},		/*	62dB	*/
{1, 1, 1, 1, 1, 1, 1}		/*	63dB	*/
};

/***************************************************************************************************
 *
 */
int pickAttnProbe(struct pci_dev *pcidev, PickeringMuxDescr *muxDesc)
{
  if(pcidev->subsystem_device != PICK_ATTN_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickAttnProbe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    return -1;
  }
  

  /* LAS0 memory address space is region 2 (BAR2) */
  muxDesc->las0addr = pci_resource_start(pcidev, 2);
  muxDesc->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "pickAttnProbe: LAS0 @ 0x%lx, size=%d\n", muxDesc->las0addr
                                                              , muxDesc->las0len);
  if (!request_mem_region(muxDesc->las0addr, muxDesc->las0len, "pickeringAttn"))
  {
    /* nothing to free*/      
    printk(KERN_CRIT "pickAttnProbe: Unable to request LAS0 @ 0x%lx, size=%d\n"
                                                               , muxDesc->las0addr
                                                               , muxDesc->las0len);
    return -1;
  }
  muxDesc->las0 = ioremap(muxDesc->las0addr, muxDesc->las0len);
  printk(KERN_INFO "pickAttnProbe: LAS0 remap @ 0x%lx (0x%p), size=%d\n", muxDesc->las0addr
                                                                         , muxDesc->las0
                                                                         , muxDesc->las0len);

  /* Allocate space to store output status */
  muxDesc->inputs = (int *)kmalloc(sizeof(int) * PICK_ATTN_MAX_CHANNEL, GFP_KERNEL);
  if(muxDesc->inputs == NULL)
  {
    printk(KERN_CRIT "pickAttnProbe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_ATTN_MAX_CHANNEL);
  
  /* Allocate space to store loop data */
  muxDesc->loopData = (int *)kmalloc(sizeof(int) * PICK_ATTN_LOOP_LEN * PICK_ATTN_NBR_LOOP, GFP_KERNEL);
  if(muxDesc->loopData == NULL)
  {
    printk(KERN_CRIT "pickAttnProbe: Unable to allocate memory for the loop data buffer\n");
    return -1;
  }
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_ATTN_LOOP_LEN * PICK_ATTN_NBR_LOOP);
  
  /* Init the hardware and the status */
  pickAttnReset(muxDesc);
  
  return 0;
}

/***************************************************************************************************
 *
 */
void  pickAttnRemove(PickeringMuxDescr *muxDesc)
{
  printk(KERN_INFO "pickAttnRemove: Removing device in bus-device: \"%d-%d\"\n", muxDesc->bus, muxDesc->device);
  
  if(muxDesc->type != PICK_ATTN_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickAttnRemove: Unsupported subdevice id (0x%X)\n", muxDesc->type);
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
int   pickAttnSetAttn(PickeringMuxDescr *muxDesc, PickeringAttnData data)
{
  if(muxDesc->type != PICK_ATTN_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickAttnSetAttn: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  printk(KERN_DEBUG "pickAttnSetAttn: Setting attn @ %d for channel %d\n", data.attn, data.channel);
  if(data.channel < 1 || data.channel > PICK_ATTN_MAX_CHANNEL)
  {
    printk(KERN_NOTICE "pickAttnSetAttn: Channel %d is out of range\n", data.channel);
    return -1;
  }
  if(data.attn < 0 || data.attn > PICK_ATTN_MAX_ATTN)
  {
    printk(KERN_NOTICE "pickAttnSetAttn: Attenuation %d is out of range\n", data.attn);
    return -1;
  }
  
  muxDesc->inputs[data.channel - 1] = data.attn;
  
  /* Set into the loop data the relays involved in the connection */
  memcpy(muxDesc->loopData + ctrlWordPosition[data.channel - 1], ctrlWords[data.attn], PICK_ATTN_NBR_ATTN * sizeof(int));
  printk(KERN_DEBUG "loopData @ %p, ctrlWordPosition = %d, ctrlWords origin @ %p, ctrlWords @ %p\n", muxDesc->loopData, ctrlWordPosition[data.channel - 1], ctrlWords, ctrlWords[data.attn]);
  printk(KERN_DEBUG "copying %d bytes from %p to %p\n", PICK_ATTN_NBR_ATTN, ctrlWords[data.attn], muxDesc->loopData + ctrlWordPosition[data.channel - 1]);
  
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_ATTN_NBR_LOOP, PICK_ATTN_LOOP_LEN);
  return 0;
}

/***************************************************************************************************
 *
 */
int   pickAttnReset(PickeringMuxDescr *muxDesc)
{
  if(muxDesc->type != PICK_ATTN_SUBSYSTEM_ID)
  {
    printk(KERN_ERR "pickAttnReset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    return -1;
  }
  /* no output connected */
  memset(muxDesc->inputs, 0, sizeof(int) * PICK_ATTN_MAX_CHANNEL);
  
  /* no relay closed */
  memset(muxDesc->loopData, 0, sizeof(int) * PICK_ATTN_LOOP_LEN * PICK_ATTN_NBR_LOOP);
  /* Send the loops data to the hardware */
  pickeringMuxSendData(muxDesc, PICK_ATTN_NBR_LOOP, PICK_ATTN_LOOP_LEN);

  return 0;
}

/***************************************************************************************************
 *
 */
int   pickAttnStatus(PickeringMuxDescr *muxDesc, int *status)
{
    int rc;
  int size;
  
  size = sizeof(int) * PICK_ATTN_MAX_CHANNEL;
  rc = copy_to_user(status, muxDesc->inputs, size);
  if(rc != 0)
  {
    printk(KERN_ERR "pickAttnStatus: copy not done correctly from %p to %p, size = %d, rc = %d.\n"
                                                                          , muxDesc->inputs
                                                                          , status
                                                                          , size
                                                                          , rc);
    return rc;
  }
  return rc;
}

/***************************************************************************************************
 *
 */
int pickAttnMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  *maxOutput = PICK_ATTN_MAX_CHANNEL;
  return 0;
}

/***************************************************************************************************
 *
 */
int pickAttnMaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  *maxInput = PICK_ATTN_MAX_CHANNEL;
  return 0;
}
