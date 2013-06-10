static char *rcsid(){return "$Id: prfmuxdriver.c,v 1.3 2003/05/12 15:32:52 sdeghaye Exp $";}
#define MODULE
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <linux/devfs_fs_kernel.h>
#include "prfmuxdriver.h"

#define PRFMUX_VENDOR      0x10B5
#define PRFMUX_DEVICE 	   0x9050
#define PRFMUX_SUB_VENDOR  0x1761
#define PRFMUX_SUB_DEVICE  0x00A7

#define MAX_SLOT 32 /* Max 32 slots a PCI bus*/

#define INPUT1_CMD 0x9
#define INPUT2_CMD 0x1
#define INPUT3_CMD 0x4
#define INPUT4_CMD 0x0

/* Prototype */
static int __devinit prfmuxProbe(struct pci_dev *pcidev, const struct pci_device_id *pciid);
static void __devexit prfmuxRemove(struct pci_dev *dev);
static int prfmuxOpen(struct inode *inode, struct file *file);
static int prfmuxRelease(struct inode *inode, struct file *file);
static int prfmuxRead(struct file *file, char *buffer, size_t count, loff_t* ppos);
static int prfmuxWrite(struct file *file, const char *buffer, size_t count, loff_t* ppos);

/* Mapping between the slot number in the CPCI backpane using the concurrent CPU and the Acqiris crate slot number*/
static int realSlot2logicalSlot[MAX_SLOT]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5, 4, 3, 2,
                                           -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
/* Module descriptor */
typedef struct 
{
  unsigned long las0addr;
  int las0len;
  void *las0;
  unsigned long las1addr;
  int las1len;    
  void *las1;
  int slot;	/* slot number of the device */
  int input;	/* input number currently connected to the output */
  devfs_handle_t file; /* DEVFS file handle for the device */
} PrfmuxDescr;

/* The devfs directory handle */
static devfs_handle_t dir;

/* File operation descriptor */						    
static struct file_operations prfmuxFileOps = 
{
  owner: THIS_MODULE,
  open: prfmuxOpen,
  release: prfmuxRelease,
  read: prfmuxRead,
  write: prfmuxWrite,
};

/* Structure for the PCI device layer */
static struct pci_device_id prfmux_idtable[] __devinitdata = 
{
  { 
    vendor: PRFMUX_VENDOR,
    device: PRFMUX_DEVICE,
    subvendor: PRFMUX_SUB_VENDOR,
    subdevice: PRFMUX_SUB_DEVICE,
    class: PCI_ANY_ID,
    class_mask: 0,
    driver_data: 0 
  },
  { 0, } 
};
	
/* Structure for the Linux PCI layer */
static struct pci_driver prfmux_driver = 
{
  name: "prfmux",
  id_table: prfmux_idtable,
  probe: prfmuxProbe,
  remove: __devexit_p(prfmuxRemove),
};
						    
/***************************************************************************************************
 *
 */
static int __devinit prfmuxProbe(struct pci_dev *pcidev, const struct pci_device_id *pciid)
{    
  int slot;
  char fileName[5];
  PrfmuxDescr *prfmux;
  
  printk(KERN_INFO "prfmux: Probing device: \"%s\"\n", pcidev->name);
  
  /* Find the slot of the device currently initialised */
  slot = PCI_SLOT(pcidev->devfn);
  if(slot >= MAX_SLOT)
  {
    printk(KERN_ERR "prfmux: Probing device: Unknown slot number %d\n", slot);
    return -1;
  }
  slot = realSlot2logicalSlot[slot];
  prfmux = (PrfmuxDescr *)kmalloc(sizeof(PrfmuxDescr), GFP_KERNEL);
  if(prfmux == NULL)
  {
    printk(KERN_ERR "prfmux: Probing device: Unable to allocate memory\n");
    return -1;
  }
 
  /* Set the driver data and init the device */
  pci_set_drvdata(pcidev, prfmux);
  pci_enable_device(pcidev);
  
  /* LAS0 memory address space is region 2 (BAR2) */
  prfmux->las0addr = pci_resource_start(pcidev, 2);
  prfmux->las0len = pci_resource_len(pcidev, 2);
  printk(KERN_INFO "prfmux: LAS0 @ 0x%lx, size=%d\n", prfmux->las0addr, prfmux->las0len);
  if (!request_mem_region(prfmux->las0addr, prfmux->las0len, "prfmux"))
  {
    /* nothing to free*/      
    printk(KERN_INFO "prfmux: Unable to request LAS0 @ 0x%lx, size=%d\n", prfmux->las0addr, prfmux->las0len);
    return -1;
  }
  prfmux->las0 = ioremap(prfmux->las0addr, prfmux->las0len);
  printk(KERN_INFO "prfmux: LAS0 remap @ 0x%lx (%p), size=%d\n", prfmux->las0addr, prfmux->las0, prfmux->las0len);

  /* LAS1 memory address space is region 3 (BAR3) */
  prfmux->las1addr = pci_resource_start(pcidev, 3);
  prfmux->las1len = pci_resource_len(pcidev, 3);
  printk(KERN_INFO "prfmux: LAS1 @ 0x%lx, size=%d\n", prfmux->las1addr, prfmux->las1len);
  if (!request_mem_region(prfmux->las1addr, prfmux->las1len, "prfmux"))
  {
    iounmap(prfmux->las0);
    release_mem_region(prfmux->las0addr, prfmux->las0len);
    printk(KERN_INFO "prfmux: Unable to request LAS1 @ 0x%lx, size=%d\n", prfmux->las1addr, prfmux->las1len);
    return -1;
  }
  prfmux->las1 = ioremap(prfmux->las1addr, prfmux->las1len);
  printk(KERN_INFO "prfmux: LAS1 remap @ 0x%lx (%p), size=%d\n", prfmux->las1addr, prfmux->las1, prfmux->las1len);

  prfmux->input = 4; /* At reset, the input 4 is connected to the output*/
  prfmux->slot = slot;
  
  /* Create the devfs inode */
  sprintf(fileName, "%s%d", PRFMUX_FILE_NAME, slot);
  prfmux->file = devfs_register(dir, fileName, DEVFS_FL_AUTO_DEVNUM, 0, 0, 
                                S_IFCHR | S_IRUGO | S_IWUGO, &prfmuxFileOps, prfmux);
  return 0;
}

/***************************************************************************************************
 *
 */
static void __devexit prfmuxRemove(struct pci_dev *pcidev)
{
  PrfmuxDescr *prfmux;
  
  printk(KERN_INFO "prfmux: Removing device: \"%s\"\n", pcidev->name);
  prfmux = pci_get_drvdata(pcidev);
  
  /* Release reserved resources */
  iounmap(prfmux->las0);
  release_mem_region(prfmux->las0addr, prfmux->las0len);
  iounmap(prfmux->las1);
  release_mem_region(prfmux->las1addr, prfmux->las1len);
  
  /* Suppress the devfs inode */
  devfs_unregister(prfmux->file);
  kfree(prfmux);
}

/***************************************************************************************************
 *
 */
static int prfmuxOpen(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "prfmux: device %d opened\n", ((PrfmuxDescr *)file->private_data)->slot);
  return 0;
}

/***************************************************************************************************
 *
 */
static int prfmuxRelease(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "prfmux: device %d closed\n", ((PrfmuxDescr *)file->private_data)->slot);
  return 0;
}

/***************************************************************************************************
 *
 */
static int prfmuxRead(struct file *file, char *buffer, size_t count, loff_t* ppos)
{
  PrfmuxDescr *prfmux;
  
  prfmux = file->private_data;
  if(prfmux == NULL)
  {
    printk(KERN_ERR "prfmux: read: No private data (device descriptor) in the file handle\n");
    return -1;
  }
  sprintf(buffer, "%d", prfmux->input);
  return 1;
}

/***************************************************************************************************
 *
 */
static int prfmuxWrite(struct file *file, const char *buffer, size_t count, loff_t* ppos)
{
  PrfmuxDescr *prfmux;
  int anInput;
  
  prfmux = file->private_data;
  if(prfmux == NULL)
  {
    printk(KERN_ERR "prfmux: write: No private data (device descriptor) in the file handle\n");
    return -1;
  }
  
  sscanf(buffer, "%d", &anInput); 
  switch(anInput)
  {
  case 1:
    *(int *)prfmux->las1 = INPUT1_CMD;
    printk(KERN_INFO "prfmux: write %d @ 0x%p\n",INPUT1_CMD, prfmux->las1);
    break;
  case 2:
    *(int *)prfmux->las1 = INPUT2_CMD;
    printk(KERN_INFO "prfmux: write %d @ 0x%p\n", INPUT2_CMD, prfmux->las1);
    break;
  case 3:
    *(int *)prfmux->las1 = INPUT3_CMD;
    printk(KERN_INFO "prfmux: write %d @ 0x%p\n", INPUT3_CMD, prfmux->las1);
    break;
  case 4:
    *(int *)prfmux->las1 = INPUT4_CMD;
    printk(KERN_INFO "prfmux: write %d @ 0x%p\n", INPUT4_CMD, prfmux->las1);
    break;
  default:
    printk(KERN_ERR "prfmux: write: Unknown input %d\n", anInput);
    return -1;
  }
  prfmux->input = anInput;
  
  return 1;
}

/***************************************************************************************************
 *
 */
int __init prfmux_init(void)
{
    int rc;
    printk(KERN_INFO "prfmux_init: START\n");
    
    /* Create the devfs directory */
    dir = devfs_mk_dir(NULL, PRFMUX_DIR_NAME, NULL);
    if(dir == 0)
    {
      printk(KERN_INFO "prfmux_init: END (devfs error)\n");    
      return -EBUSY;
    }
    
    rc = pci_module_init(&prfmux_driver);
    if (rc)
    {
      printk(KERN_INFO "prfmux_init: END (Error=%d)\n", rc);    
      return rc;
    }
    printk(KERN_INFO "prfmux_init: END (Ok)\n");    
    return 0;
}
 
/***************************************************************************************************
 *
 */
void __exit prfmux_cleanup(void)
{
    pci_unregister_driver(&prfmux_driver);
   
    /* Suppress the devfs directory */
    devfs_unregister(dir);
}

/* Definition of the init & exit functions for the module */ 
module_init(prfmux_init);
module_exit(prfmux_cleanup);

MODULE_DESCRIPTION("Pickering RF MUX 4 to 1 PCI driver");
MODULE_AUTHOR("S. Deghaye from a D. P. Bovet and M. Cesati's skeleton");
MODULE_LICENSE("GPL");
