static __attribute__((unused)) char *rcsid = "$Id: pickeringmuxmain.c,v 1.10 2012/04/18 13:13:31 sdeghaye Exp $";

#ifndef MODULE
#define MODULE
#endif

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "pickeringmuxdrv.h"
#include "pickering4to1.h"
#include "pickering9to8.h"
#include "pickering16to16.h"
#include "pickering16to16a.h"
#include "pickering22to8.h"
#include "pickeringattn.h"

#define PICKERINGMUX_VENDOR      0x10B5
#define PICKERINGMUX_DEVICE 	   0x9050
#define PICKERINGMUX_SUB_VENDOR  0x1761
/* We support several modules. 
   Whether we support the module or not is decided in the probe function */
#define PICKERINGMUX_SUB_DEVICE  PCI_ANY_ID

/* Prototypes */
static int  __init    pickeringMuxInit(void);
static void __exit    pickeringMuxCleanup(void);
static int  __devinit pickeringMuxProbe(struct pci_dev *pcidev, const struct pci_device_id *pciid);
static void __devexit pickeringMuxRemove(struct pci_dev *dev);
static int            pickeringMuxOpen(struct inode *inode, struct file *file);
static int            pickeringMuxRelease(struct inode *inode, struct file *file);
static long           pickeringMuxIoctl(struct file *file, unsigned int cmd, unsigned long arg);
static int            pickeringMuxConnect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data);
static int            pickeringMuxDisconnect(PickeringMuxDescr *muxDesc, int output);
static int            pickeringMuxReset(PickeringMuxDescr *muxDesc);
static int            pickeringMuxMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput);
static int            pickeringMuxMaxInput(PickeringMuxDescr *muxDesc, int *maxInput);
static int            pickeringMuxStatus(PickeringMuxDescr *muxDesc, int *status);
static int            pickeringMuxWidth(PickeringMuxDescr *muxDesc, int *width);
static int            pickeringSetAttn(PickeringMuxDescr *muxDesc,  PickeringAttnData data);
#if 0
static void           dumpMemory(void *addr, int size);
#endif
    
/* File operation descriptor */						    
static struct file_operations pickeringMuxFileOps = 
{
  .owner = THIS_MODULE,
  .open = pickeringMuxOpen,
  .release = pickeringMuxRelease,
  .unlocked_ioctl = pickeringMuxIoctl,
};

/* Major number which will be dyna,ically allocated */
static unsigned int pickeringMuxMajor = 0;

/* Structure for the PCI device layer */
static struct pci_device_id pickeringMuxIdtable[] __devinitdata = 
{
  { 
    .vendor = PICKERINGMUX_VENDOR,
    .device = PICKERINGMUX_DEVICE,
    .subvendor = PICKERINGMUX_SUB_VENDOR,
    .subdevice = PICKERINGMUX_SUB_DEVICE,
    .class = PCI_ANY_ID,
    .class_mask = 0,
    .driver_data = 0 
  },
  { 0, } 
};
	
/* Structure for the Linux PCI layer */
static struct pci_driver pickeringMuxDriver = 
{
  .name = PICKERINGMUX_DIR_NAME,
  .id_table = pickeringMuxIdtable,
  .probe = pickeringMuxProbe,
  .remove = __devexit_p(pickeringMuxRemove),
};
  
/* Definition of the init & exit functions for the module */ 
module_init(pickeringMuxInit);
module_exit(pickeringMuxCleanup);
						    
/***************************************************************************************************
 *
 */
static int __devinit pickeringMuxProbe(struct pci_dev *pcidev, const struct pci_device_id *pciid)
{    
  PickeringMuxDescr *muxDesc;
  int rc = 0;
  unsigned char bus;
  unsigned int device;
  dev_t muxDev;
  
#ifdef CONFIG_DEVFS_FS
  printk(KERN_INFO "pickeringMuxProbe: Probing device: \"%s\" (type %X)\n", pcidev->pretty_name, pcidev->subsystem_device);
#else
  printk(KERN_INFO "pickeringMuxProbe: Probing device: \"%s\" (type %X)\n", pci_name(pcidev), pcidev->subsystem_device);
#endif
  
  /* General part (pre-specific) */
  muxDesc = (PickeringMuxDescr *)kmalloc(sizeof(PickeringMuxDescr), GFP_KERNEL);
  if(muxDesc == NULL)
  {
    printk(KERN_CRIT "pickeringMuxProbe: Unable to allocate memory\n");
    return -1;
  }
  memset(muxDesc, 0, sizeof(PickeringMuxDescr));
  
  /* Find the slot of the device currently initialised */
  bus = pcidev->bus->number;
  device = PCI_SLOT(pcidev->devfn);
  muxDesc->bus = bus;
  muxDesc->device = device;
  muxDesc->type = pcidev->subsystem_device;
 
  /* Set the driver data and init the device */
  pci_set_drvdata(pcidev, muxDesc);
  pci_enable_device(pcidev);
  
  /* Specific part */
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1Probe(pcidev, muxDesc);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8Probe(pcidev, muxDesc);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16Probe(pcidev, muxDesc);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aProbe(pcidev, muxDesc);
    break;    
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8Probe(pcidev, muxDesc);
    break;      
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnProbe(pcidev, muxDesc);
    break;
  default:
    printk(KERN_ERR "pickeringMuxProbe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    kfree(muxDesc);
    return -1;
  }
  
  /* General part (post-specific) */
  if(rc != 0)
  {
    kfree(muxDesc);    
    return rc;
  }
  
  /* Create a device from the bus/PCI device no */
  muxDev = MKDEV(pickeringMuxMajor, bus*MAX_SLOT + device);
#ifdef CONFIG_DEVFS_FS
  /* Create the devfs inode */
  devfs_mk_cdev(muxDev, S_IFCHR|S_IRUGO|S_IWUGO, PICKERINGMUX_FILE_TEMPLATE, bus, device);
#endif
  /* Create a cdev */
  cdev_init(&muxDesc->charDev, &pickeringMuxFileOps);
  muxDesc->charDev.owner = THIS_MODULE;
	rc = cdev_add(&muxDesc->charDev, muxDev, 1);
	if (rc) 
  {
	  printk(KERN_CRIT "pickeringMuxInit: unable to register character device (%d)\n", rc);
    kfree(muxDesc);
		return rc;
	}
  return 0;
}

/***************************************************************************************************
 *
 */
static void __devexit pickeringMuxRemove(struct pci_dev *pcidev)
{
  PickeringMuxDescr *muxDesc;
  
#ifdef CONFIG_DEVFS_FS
  printk(KERN_INFO "pickeringMuxRemove: Removing device: \"%s\"\n", pcidev->pretty_name);
#else
  printk(KERN_INFO "pickeringMuxRemove: Removing device: \"%s\"\n", pci_name(pcidev));
#endif
  muxDesc = pci_get_drvdata(pcidev);
  
  /* Suppress the cdev */
  cdev_del(&muxDesc->charDev);
  
#ifdef CONFIG_DEVFS_FS
  /* Suppress the devfs inode */
  devfs_remove(PICKERINGMUX_FILE_TEMPLATE, muxDesc->bus, muxDesc->device);
#endif
  
  /* Specific part */
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    pickMux4to1Remove(muxDesc);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    pickMux9to8Remove(muxDesc);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    pickMux16to16Remove(muxDesc);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    pickMux16to16aRemove(muxDesc);
    break;
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    pickMux22to8Remove(muxDesc);
    break;    
  case PICK_ATTN_SUBSYSTEM_ID:
    pickAttnRemove(muxDesc);
    break;
  default:
    printk(KERN_ERR "pickeringMuxProbe: Unsupported subdevice id (0x%X)\n", pcidev->subsystem_device);
    break;
  }
  
  /* Finaly, release the memory */
  kfree(muxDesc);
}

/***************************************************************************************************
 *
 */
static int pickeringMuxOpen(struct inode *inode, struct file *file)
{
  PickeringMuxDescr *muxDesc;
  muxDesc = container_of(inode->i_cdev, PickeringMuxDescr, charDev);
  file->private_data = muxDesc;
   
  printk(KERN_DEBUG "pickeringMuxOpen: device %d-%d opened\n", ((PickeringMuxDescr *)file->private_data)->bus, ((PickeringMuxDescr *)file->private_data)->device);
  return 0;
}

/***************************************************************************************************
 *
 */
static int pickeringMuxRelease(struct inode *inode, struct file *file)
{
  printk(KERN_DEBUG "pickeringMuxRelease: device %d-%d closed\n", ((PickeringMuxDescr *)file->private_data)->bus, ((PickeringMuxDescr *)file->private_data)->device);
  return 0;
}

/***************************************************************************************************
 *
 */
static long pickeringMuxIoctl(struct file *file, unsigned int cmd, unsigned long arg)
{
  PickeringMuxDescr *muxDesc;
  PickeringMuxConnectData connectData;
  PickeringAttnData attnData;
  int rc;
  int data;
  
  muxDesc = file->private_data;
  if(muxDesc == NULL)
  {
    printk(KERN_ERR "pickeringMuxWrite: write: No private data (device descriptor) in the file handle\n");
    return -1;
  }
  
  switch(cmd)
  {
  case PICKERINGMUX_CONNECT:
    rc = copy_from_user(&connectData, (PickeringMuxConnectData *)arg, sizeof(PickeringMuxConnectData));
    if(rc != 0)
    { 
      break;
    }
    rc = pickeringMuxConnect(muxDesc, connectData);
    break;
  case PICKERINGMUX_DISCONNECT:
    rc = get_user(data, (int *)arg);
    if(rc == -EFAULT)
    { 
      break;
    }
    rc = pickeringMuxDisconnect(muxDesc, data);
    break;
  case PICKERINGMUX_RESET:
    rc = pickeringMuxReset(muxDesc);
    break;
  case PICKERINGMUX_MAXOUTPUT:
    rc = pickeringMuxMaxOutput(muxDesc, &data);
    if(rc != 0)
    { 
      break;
    }
    rc = put_user(data, (int *)arg);
    break;
  case PICKERINGMUX_MAXINPUT:
    rc = pickeringMuxMaxInput(muxDesc, &data);
    if(rc != 0)
    { 
      break;
    }
    rc = put_user(data, (int *)arg);
    break;
  case PICKERINGMUX_STATUS:
    rc = pickeringMuxStatus(muxDesc, (int *)arg);
    break;
  case PICKERINGMUX_WIDTH:
    rc = pickeringMuxWidth(muxDesc, (int *)arg);
    break; 
  case PICKERING_SET_ATTN:
    rc = copy_from_user(&attnData, (PickeringAttnData *)arg, sizeof(PickeringAttnData));
    if(rc != 0)
    { 
      break;
    }
    rc = pickeringSetAttn(muxDesc, attnData);
    break;
  default:
    printk(KERN_ERR "pickeringMuxProbe: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }  
  return rc;
}

/***************************************************************************************************
 *
 */
static int __init pickeringMuxInit(void)
{
    int rc;
    dev_t firstDev;
    
    printk(KERN_INFO "pickeringMuxInit: START\n");
    
#ifdef CONFIG_DEVFS_FS
    /* Create the devfs directory */
    rc = devfs_mk_dir(PICKERINGMUX_DIR_NAME);
    if(rc != 0)
    {
      printk(KERN_CRIT "pickeringMuxInit: END (devfs error = %d)\n", rc);    
      return -EBUSY;
    }
#endif
   
    /* Allocate device number */
    rc = alloc_chrdev_region(&firstDev, 0, MAX_BUS * MAX_SLOT, PICKERINGMUX_DIR_NAME);
    if(rc != 0)
    {
      /* Suppress the devfs directory */
#ifdef CONFIG_DEVFS_FS
      nmndevfs_remove(PICKERINGMUX_DIR_NAME);
#endif
      printk(KERN_CRIT "pickeringMuxInit: END (alloc_chrdev_region error = %d)\n", rc);    
      return -EBUSY;
    }
    
    /* Get the newly allocated Major number */
    pickeringMuxMajor = MAJOR(firstDev);
    
    /* Init the hardware */
    rc = pci_register_driver(&pickeringMuxDriver);
    if (rc != 0)
    {
      /* Free the chrdev region */
      unregister_chrdev_region(firstDev, MAX_BUS * MAX_SLOT);
#ifdef CONFIG_DEVFS_FS
      /* Suppress the devfs directory */
      devfs_remove(PICKERINGMUX_DIR_NAME);
#endif
      printk(KERN_ERR "pickeringMuxInit: END (Error=%d)\n", rc);    
      return rc;
    }
    printk(KERN_INFO "pickeringMuxInit: END (Ok)\n");    
    return 0;
}
 
/***************************************************************************************************
 *
 */
static void __exit pickeringMuxCleanup(void)
{
    pci_unregister_driver(&pickeringMuxDriver);
    
    /* Free the chrdev region */
    unregister_chrdev_region(MKDEV(pickeringMuxMajor, 0), MAX_BUS * MAX_SLOT);
   
#ifdef CONFIG_DEVFS_FS
    /* Suppress the devfs directory */
    devfs_remove(PICKERINGMUX_DIR_NAME);
#endif
}

/***************************************************************************************************
 *
 */
static int pickeringMuxConnect(PickeringMuxDescr *muxDesc, PickeringMuxConnectData data)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1Connect(muxDesc, data);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8Connect(muxDesc, data);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16Connect(muxDesc, data);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aConnect(muxDesc, data);
    break;    
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8Connect(muxDesc, data);
    break;
  default:
    printk(KERN_ERR "pickeringMuxConnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;  
}

/***************************************************************************************************
 *
 */
static int pickeringMuxDisconnect(PickeringMuxDescr *muxDesc, int output)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1Disconnect(muxDesc, output);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8Disconnect(muxDesc, output);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16Disconnect(muxDesc, output);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aDisconnect(muxDesc, output);
    break;
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8Disconnect(muxDesc, output);
    break;
  default:
    printk(KERN_ERR "pickeringMuxDisconnect: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}

/***************************************************************************************************
 *
 */
static int pickeringMuxReset(PickeringMuxDescr *muxDesc)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1Reset(muxDesc);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8Reset(muxDesc);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16Reset(muxDesc);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aReset(muxDesc);
    break;    
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8Reset(muxDesc);
    break;
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnReset(muxDesc);
    break;
  default:
    printk(KERN_ERR "pickeringMuxReset: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}

/***************************************************************************************************
 *
 */
static int pickeringMuxMaxOutput(PickeringMuxDescr *muxDesc, int *maxOutput)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1MaxOutput(muxDesc, maxOutput);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8MaxOutput(muxDesc, maxOutput);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16MaxOutput(muxDesc, maxOutput);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aMaxOutput(muxDesc, maxOutput);
    break;
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8MaxOutput(muxDesc, maxOutput);
    break;
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnMaxOutput(muxDesc, maxOutput);
    break;    
  default:
    printk(KERN_ERR "pickeringMuxMaxOutput: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc; 
}

/***************************************************************************************************
 *
 */
static int pickeringMuxMaxInput(PickeringMuxDescr *muxDesc, int *maxInput)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1MaxInput(muxDesc, maxInput);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8MaxInput(muxDesc, maxInput);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16MaxInput(muxDesc, maxInput);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aMaxInput(muxDesc, maxInput);
    break;    
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8MaxInput(muxDesc, maxInput);
    break;
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnMaxInput(muxDesc, maxInput);
    break;
  default:
    printk(KERN_ERR "pickeringMuxMaxInput: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}

/***************************************************************************************************
 *
 */
static int pickeringMuxStatus(PickeringMuxDescr *muxDesc, int *status)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
    rc = pickMux4to1Status(muxDesc, status);
    break;
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
    rc = pickMux9to8Status(muxDesc, status);
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
    rc = pickMux16to16Status(muxDesc, status);
    break;
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    rc = pickMux16to16aStatus(muxDesc, status);
    break;
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    rc = pickMux22to8Status(muxDesc, status);
    break;    
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnStatus(muxDesc, status);
    break;
  default:
    printk(KERN_ERR "pickeringMuxStatus: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}


/***************************************************************************************************
 * FUNCTION:    pickeringMuxWidth.
 * DESCRIPTION: 'ioctl' method to obtain hardware module 
 *		width (in terms of slots).
 * RETURNS:	0   - if succeed.
 *		-1  - in case of failure.
 */
static int pickeringMuxWidth(PickeringMuxDescr *muxDesc, int *width)
{
  int rc = 0;
  
  switch (muxDesc->type) 
  {
  case PICK_MUX_4_TO_1_SUBSYSTEM_ID:
  case PICK_MUX_9_TO_8_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_1_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_2_SUBSYSTEM_ID:
  case PICK_MUX_22_TO_8_3_SUBSYSTEM_ID:
    *width = 1;			/* occupy one slot */
    break;
  case PICK_MUX_16_TO_16_SUBSYSTEM_ID:
  case PICK_MUX_16_TO_16A_SUBSYSTEM_ID:
    *width = 2;			/* occupy two slots */
    break;
  default:
    printk(KERN_ERR "pickeringMuxStatus: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}

/***************************************************************************************************
 *
 */
static int pickeringSetAttn(PickeringMuxDescr *muxDesc, PickeringAttnData data)
{
  int rc = 0;
  
  switch(muxDesc->type)
  {
  case PICK_ATTN_SUBSYSTEM_ID:
    rc = pickAttnSetAttn(muxDesc, data);
    break;
  default:
    printk(KERN_ERR "pickeringSetAttn: Unsupported subdevice id (0x%X)\n", muxDesc->type);
    rc = -1;
    break;
  }
  return rc;
}


/***************************************************************************************************
 *
 */
void pickeringMuxSendData(PickeringMuxDescr *muxDesc, int nbrLoop, int loopLen)
{
  int loop;
  int bit;
  int data;
  int *ctrlReg;
  int *loopData;
  
  printk(KERN_DEBUG "pickeringMuxSendData: sending %d ints to mux in slot %d-%d \n", nbrLoop * loopLen, muxDesc->bus, muxDesc->device);  
  
  ctrlReg = (int *)muxDesc->las0;
  loopData = muxDesc->loopData;
  
  for(loop = 1; loop <= nbrLoop; loop++)
  {
    for(bit = 0; bit < loopLen; bit++)
    {
      data = PICK_MUX_OE_SET; /* OE = 1 */
      data |= loopData[bit + loopLen * (loop - 1)] << PICK_MUX_DATA_POS;
      *ctrlReg = data;
      udelay(PICK_MUX_CLK_PERIOD);
      data |= 1 << PICK_MUX_CLK_POS;
      *ctrlReg = data;
      udelay(PICK_MUX_CLK_PERIOD);
    }
    /* Strobe the data*/
    data = PICK_MUX_OE_SET;
    switch(loop)
    {
    case 1:
      data |= 1 << PICK_MUX_STRB1_POS;
      break;
    case 2:
      data |= 1 << PICK_MUX_STRB2_POS;
      break;
    default:
      printk(KERN_ERR "Pickering Serial Architecture: Unsupported loop number %d\n", loop);
      break;
    }
    *ctrlReg = data;
    udelay(PICK_MUX_CLK_PERIOD);
  }
  /* Clear the last strobe */
  *ctrlReg = PICK_MUX_OE_SET;
}

#if 0
/*******************************************************************************
 *
 */
void dumpMemory (void *addr, int size)
{
    int i, ligne = 0, pos;
    char decod[20], curByte, txt[100];
    
    printk(KERN_DEBUG "\n>>>>>>>> Memory Dump\n\n");
    while(size > ligne * 16)
    {
        memset(decod, 0, sizeof(decod));
        memset(txt, 0, sizeof(txt));
        pos = 0;
        
        pos = sprintf(txt, "%p : ", addr);
        for(i = 0; i < 16; i++)
        {
            if((ligne * 16) + i < size)
            {
                curByte = ((char*)addr)[i];
                pos += sprintf(txt + pos, "%.2X ",(unsigned char)curByte);
                if( 0x1F < curByte && curByte < 0x7F)
                    decod[i] = curByte;
                else
                    decod[i] = '.'; 
            }
            else
                pos += sprintf(txt + pos, "?? "); 
                
            if(!((i+1)% 4))
                pos += sprintf(txt + pos, " ");
        }
        pos += sprintf(txt + pos, "|%s\n", decod);
        printk(KERN_DEBUG "%s\n", txt);
        ligne++;
        addr = (char *)addr + 16;
    }
    printk(KERN_DEBUG "\n");
}
#endif

MODULE_DESCRIPTION("Pickering MUX Linux driver");
MODULE_AUTHOR("S. Deghaye");
MODULE_LICENSE("GPL");
