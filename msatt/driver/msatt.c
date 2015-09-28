#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/limits.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/uaccess.h>        /* copy_*_user */
#include "msatt.h"

#define MAX_DEVICES 16
#define DRIVER_NAME "msatt"
#define PFX DRIVER_NAME ": "

#define MAX_ATT_ALL 0x3333

/* Module parameters */
static int lun[MAX_DEVICES];
static int num_lun;
module_param_array(lun, int, &num_lun, S_IRUGO);
MODULE_PARM_DESC(lun, "Logical Unit Number");

static int bus[MAX_DEVICES];
static int num_bus;
module_param_array(bus, int, &num_bus, S_IRUGO);
MODULE_PARM_DESC(bus, "PCI Bus Number");

static int slot[MAX_DEVICES];
static int num_slot;
module_param_array(slot, int, &num_slot, S_IRUGO);
MODULE_PARM_DESC(slot, "Slot Number on PCI Bus");

/* Description of a msatt module */
struct msatt_t {
	int		lun;
	int		bus;
	int		slot;
	struct pci_dev 	*pci_dev;
	void __iomem	*regmem;
	void __iomem	*relay_cmd;
	unsigned int	nchan; /* Number of channels */
	spinlock_t	lock; /* To lock the registry */	
};

/* Static device table */
static struct msatt_t devices[MAX_DEVICES];
static int num_devices;

static struct cdev 	*cdev;
static dev_t 		dev;

static struct pci_device_id msatt_tbl[] = {
	{ MSATT_VENDOR_ID, MSATT_DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	/* required last entry */
	{0,}
};
/* MODULE_DEVICE_TABLE(pci, msatt_tbl); */

/*
static struct msatt_t *find_device_by_lun(int lun)
{
	int i;

	for (i = 0; i < num_devices; i++)
		if (devices[i].lun == lun)
			return &devices[i];
	return NULL;
}
*/

/*
static irqreturn_t msatt_interrupt (int irq, void *dev_id)
{
	printk (PFX "Ok, interrupt %d handled\n", irq);

	return IRQ_HANDLED;
}
*/


static struct msatt_t *find_device_config(struct pci_dev *dev)
{
	int bus  = dev->bus->number;
	int slot = PCI_SLOT(dev->devfn);
	int i;

	printk(KERN_INFO PFX
		"Trying to find device on bus %d and slot %d\n", bus, slot);

	/* Find the device in config table */
	for (i = 0; i < num_devices; i++) {
		struct msatt_t *this_device = &devices[i];
		if (this_device->bus  == bus && this_device->slot == slot)
			return this_device;
	}
	return NULL;
}

static int map_registers(struct msatt_t *entry)
{
	unsigned long 	len;
	u16		description;
	u16		version;
	struct pci_dev	*pcid = entry->pci_dev;

	if (pci_request_region(pcid, MSATT_REGS_BAR, DRIVER_NAME) < 0) {
		printk(PFX "Request regions failed\n");
		goto failed;
	}

	if ((pci_resource_flags(pcid, MSATT_REGS_BAR) & IORESOURCE_MEM) == 0) {
		printk(KERN_ERR PFX
			"No I/O resource at BAR #%d\n", MSATT_REGS_BAR);
		goto failed_map;
	}

	len = pci_resource_len(pcid, MSATT_REGS_BAR);
	if (len < MSATT_REGS_BAR_SIZE) {
		printk(KERN_ERR PFX "wrong BAR#%d size = %ld, expected %d\n",
					MSATT_REGS_BAR,
					len,
					MSATT_REGS_BAR_SIZE);
		goto failed_map;
	}

	/* Mapping BAR with IO register */
	entry->regmem = ioremap(pci_resource_start(pcid, MSATT_REGS_BAR), len);
	if (entry->regmem == NULL) {
		printk(KERN_ERR PFX "could not map BAR#%d\n", MSATT_REGS_BAR);
		goto failed_map;
	}

	/* Check for registers */
	/* Read only register at 0x000. Version and description*/
	description = ioread16(entry->regmem);
	version = description & 0x00FF;
	
	if ((version != MSATT_VERSION_1) && (version != MSATT_VERSION_2)) {
		printk(KERN_ERR PFX "no valid version found\n");
		goto failed_regs;
	}
	
	/* Only checks high part */
	description = description & 0xFF00;
	if ((description != MSATT_N2) && (description != MSATT_N4)) {
		printk(KERN_ERR PFX
			"no valid description (0x%2x) found\n", description);
		goto failed_regs;
	}
	
	/* Get number of channels from description value */
	entry->nchan = description >> 8;
	printk(KERN_ERR PFX "MSATTN%d version %d found\n",
				entry->nchan, version);
	/* Get IO register address */
	entry->relay_cmd = entry->regmem + MSATT_COMMAND_REG;

	/* Client request: All channels to 40dB by default */
	/* Btw, this is the default hw behaviour in version 2 */
	iowrite16(MAX_ATT_ALL, entry->relay_cmd);

	return 1;
failed_regs:
	iounmap(entry->regmem);
failed_map:
	pci_release_region(entry->pci_dev, MSATT_REGS_BAR);
failed:
	return 0;
}

static int msatt_probe(struct pci_dev *dev,
					const struct pci_device_id *ent)
{
	struct msatt_t *cfg_entry;

	printk(PFX "Probe function: Enabling device\n");

	/* Check if static config is present */
	cfg_entry = find_device_config(dev);
	if (!cfg_entry) {
				printk(KERN_INFO PFX
				"no config data for bus = %d, slot %d\n",
				dev->bus->number, PCI_SLOT(dev->devfn));
				goto failed_enable;
	}
	printk(KERN_INFO PFX
		"configuring device at bus = %d, slot %d\n",
		cfg_entry->bus, cfg_entry->slot);

	/* Enable device */
	if (pci_enable_device(dev) < 0) {
		printk(KERN_ERR PFX "enable device failed\n");
		goto failed_enable;
	}
	printk(KERN_INFO PFX "device enabled\n");

	cfg_entry->pci_dev = dev;

	/* Map onboard registers*/
	if (!map_registers(cfg_entry)) {
		printk(KERN_ERR PFX "could not map registers\n");
		goto failed_map;
	}

	/* All is ok! */
	printk(KERN_INFO PFX "configured device  "
		"lun = %d, bus = %d, slot = %d \n",
		cfg_entry->lun,
		cfg_entry->bus, cfg_entry->slot);

	return 0;

failed_map:
	pci_disable_device(dev);
failed_enable:
	return -ENODEV;

}

static void msatt_remove(struct pci_dev *dev)
{
	printk(PFX "Releasing PCI device\n");

	/* Drivers should call pci_release_region AFTER pci_disable_device */
	pci_disable_device(dev);
	pci_release_region(dev, MSATT_REGS_BAR);

	return;
}

static int lun_to_index(int lun)
{
	int i;

	for (i = 0; i < num_devices; i++) {
		if (devices[i].lun == lun)
			return i;
	}

	return -1;
}


static int msatt_open(struct inode *ino, struct file  *filp)
{
	unsigned int lun = iminor(ino);
	int idx = -1;

	idx = lun_to_index(lun);

	if (idx < 0) {
		printk(KERN_ERR PFX "could not open, bad lun %d\n", lun);
		return -ENODEV;
	}

	filp->private_data = &devices[idx];
	spin_lock_init(&devices[idx].lock);
	return 0;
}

static int msatt_release(struct inode *ino, struct file  *filp)
{
	return 0;
}

int write_relay_value(struct msatt_t *dev, int channel, int value)
{
	u16	relay_value;
	u16	mask;

	if ((value != 0) && /* No attenuation */
		(value != 1) && /* 20 dB */
		(value != 3)) { /* 40 dB */

		printk(KERN_ERR PFX "Set: Invalid value %#x\n", value);
			return -EINVAL;
	}

	spin_lock(&dev->lock);
	/* Read old value */
	relay_value = ioread16(dev->relay_cmd);
	/* Build reset mask */
	mask = ~(3 << (4 *channel));
	/* Reset desired relay */
	relay_value &= mask;
	/* Set the new value */
	mask = value << (4 * channel);
	relay_value |= mask;

	iowrite16(relay_value, dev->relay_cmd);
	spin_unlock(&dev->lock);

	return 0;
}

static long msatt_ioctl(struct file *fp,
			unsigned int op,
			unsigned long arg)
{
	u16 relay_value;
	struct msatt_params myarg, *myargp = &myarg;
	int sizeop = _IOC_SIZE(op);

	struct msatt_t *dev = fp->private_data;
	int nchan = dev->nchan;
	int *nchanp = &nchan;

	if (op == IOCTL_SET_CMD || op == IOCTL_GET_CMD) {
		if (copy_from_user(myargp, (const void __user *)arg, sizeof(myarg))) {
			printk(KERN_ERR PFX "can not copy data \
						 from user space\n");
			return -EINVAL;
		}
		
		/* Client request: channels begins at 1. We use [0,N] here*/
		myarg.channel--;

		if (myarg.channel < 0 || myarg.channel > nchan) {
			printk(KERN_ERR PFX "Set: invalid channel number %d \n",
						myarg.channel);
		return -EINVAL;
		}
	}


	switch (op) {

	case IOCTL_SET_CMD:
		write_relay_value(dev, myarg.channel, myarg.value);
		break;
	case IOCTL_GET_CMD:
		spin_lock(&dev->lock);
		relay_value = ioread16(dev->relay_cmd);
		spin_unlock(&dev->lock);
		
		myarg.value = relay_value >> (4 * myarg.channel);
		myarg.value &= 0x0F;	/* FIXME: this should be 0x3, not 0xf */

		if (copy_to_user((void __user *)arg, myargp, sizeop)) {
			printk(KERN_ERR PFX "can not copy to user space\n");
			return -EINVAL;
		}
		break;
	case IOCTL_GET_NUMCHAN:
		if (copy_to_user((void __user *)arg, nchanp, sizeop)) {
			printk(KERN_ERR PFX "can not copy to user space\n");
			return -EINVAL;
		}
		break;
	default:
		return -ENOTTY;
	}

	return 0;
}


/* Structure that declares the usual file */
/* access functions */
static const struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open 		= msatt_open,
	.release	= msatt_release,
	.unlocked_ioctl	= msatt_ioctl
 };

static struct pci_driver msatt_driver = {
	.name		= DRIVER_NAME,
	.probe		= msatt_probe,
	.remove		= msatt_remove,
	.id_table	= msatt_tbl
};

static int __init msatt_init(void)
{
	int i = 0;
	int err;

	int major = 0;

	printk(KERN_INFO PFX "initializing driver\n");
	if (num_lun < 0 || num_lun >= MAX_DEVICES) {
		printk(KERN_ERR PFX
		"invalid number of configured devices %d\n", num_lun);
		goto failed_init;
	} else {
		printk(KERN_INFO PFX
			"Start configuration for %d device(s)\n", num_lun);
	}

	for (i = 0; i < num_lun; i++) {
		struct msatt_t *newdev = &devices[i];

		newdev->lun = lun[i];
		newdev->bus = bus[i];
		newdev->slot = slot[i];
	}
	num_devices = i;

	if (pci_register_driver(&msatt_driver) != 0) {
		printk(KERN_ERR PFX "Error installing PCI driver\n");
		goto failed_init;
	} else {
		printk(KERN_INFO PFX "Driver registered\n");
	}

	err = alloc_chrdev_region(&dev, 0, MAX_DEVICES, DRIVER_NAME);

	if (err < 0) {
		printk(KERN_ERR PFX "cannot register chrdev region\n");
		goto error_cdev;
	}

	major = MAJOR(dev);

	cdev = cdev_alloc();
	if (cdev == NULL)
		goto error_cdev;

	cdev_init(cdev, &fops);
	cdev->owner = THIS_MODULE;
	cdev->ops = &fops;

	err = cdev_add(cdev, dev, MAX_DEVICES);
	if (err < 0) {
		printk(KERN_ERR PFX "cannot add character device\n");
		goto error_cdev;
	}

	return 0;

error_cdev:
	pci_unregister_driver(&msatt_driver);

failed_init:
	printk(KERN_ERR PFX "module exiting\n");
	return -1;

}

static void __exit msatt_exit(void)
{
	unregister_chrdev_region(dev, MAX_DEVICES);
	cdev_del(cdev);
	pci_unregister_driver(&msatt_driver);

	printk(PFX "Exit module\n");
}

module_init(msatt_init);
module_exit(msatt_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Magenta SYS cPCI attenuator driver");
MODULE_AUTHOR("Luis Fernando Ruiz Gago");
MODULE_VERSION("1.0");
