/**
 * carrier_common.h
 *
 * driver for the carrier TEWS TPCI-200 
 * Copyright (c) 2009 Nicolas Serafini, EIC2 SA
 * Copyright (c) 2010,2011 Samuel Iglesias Gonsalvez <siglesia@cern.ch>
 * Copyright (c) 2013 Luis Fernando Ruiz Gago <lfruiz@cern.ch>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.* Released under the GPL v2. (and only v2, not any later version)
 */

#include <asm/io.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <linux/slab.h>

#include "vmebus.h"
#include "tvme200.h"

#define MAX_CARRIER 16
#define PFX "tvme200: "
#define ENDLIST -1

static int lun[MAX_CARRIER];
static int num_lun;
module_param_array(lun, int, &num_lun, S_IRUGO);

static int mod_address_ioid[MAX_CARRIER];
static int num_address_ioid;
module_param_array(mod_address_ioid, int, &num_address_ioid, S_IRUGO);
static int base_address_ioid[MAX_CARRIER];
static int num_base_address_ioid;
module_param_array(base_address_ioid, int, &num_base_address_ioid, S_IRUGO);
static int data_width_ioid[MAX_CARRIER];
static int num_data_width_ioid;
module_param_array(data_width_ioid, int, &num_data_width_ioid, S_IRUGO);
static int wind_size_ioid[MAX_CARRIER];
static int num_wind_size_ioid;
module_param_array(wind_size_ioid, int, &num_wind_size_ioid, S_IRUGO);

static int mod_address_mem[MAX_CARRIER];
static int num_address_mem;
module_param_array(mod_address_mem, int, &num_address_mem, S_IRUGO);
static int base_address_mem[MAX_CARRIER];
static int num_base_address_mem;
module_param_array(base_address_mem, int, &num_base_address_mem, S_IRUGO);
static int data_width_mem[MAX_CARRIER];
static int num_data_width_mem;
module_param_array(data_width_mem, int, &num_data_width_mem, S_IRUGO);
static int wind_size_mem[MAX_CARRIER];
static int num_wind_size_mem;
module_param_array(wind_size_mem, int, &num_wind_size_mem, S_IRUGO);

static int irq[MAX_CARRIER * 4];
static unsigned int num_irq;
module_param_array(irq, int, &num_irq, S_IRUGO);
MODULE_PARM_DESC(irq, "IRQ vectors for mezzanines, 4 for each board. Expected -1 if slot empty.");

#define TVME200_SPACE_NUM 3 

static const size_t tvme200_space_size[TVME200_SPACE_NUM] = {
        [IPACK_IO_SPACE]    = TVME200_IO_SPACE_SIZE,
        [IPACK_ID_SPACE]    = TVME200_ID_SPACE_SIZE,
        [IPACK_INT_SPACE]   = TVME200_INT_SPACE_SIZE,
};

static const size_t tvme200_space_interval[TVME200_SPACE_NUM] = {
        [IPACK_IO_SPACE]    = TVME200_IO_SPACE_INTERVAL,
        [IPACK_ID_SPACE]    = TVME200_ID_SPACE_INTERVAL,
        [IPACK_INT_SPACE]   = TVME200_INT_SPACE_INTERVAL,
};

static int data_width[] = { VME_D8, VME_D16, ENDLIST };
static int a16_modifiers[] = { 0x29, 0x2D, ENDLIST };
static int a24_modifiers[] = { 0x39, 0x3A, 0x3D, 0x3E, ENDLIST };
static int a32_modifiers[] = { 0x09, 0x0A, 0x0D, 0x0E, ENDLIST };

static int carrier_mem_size[] = { TVME200_MEM_128k, TVME200_MEM_256k,
	TVME200_MEM_512k, TVME200_MEM_1M,
	TVME200_MEM_2M, TVME200_MEM_4M,
	TVME200_MEM_8M, TVME200_MEM_32M, ENDLIST };

static struct tvme200_board* carrier_boards = NULL;

static int tvme200_free_irq_slot(struct tvme200_board *tvme200, int slot);

static int set_ioid_params(struct tvme200_board *tvme200, struct vme_addr_space *ioid_space)
{
	int i = 0;
	struct device *dev=tvme200->info->dev;

	dev_info(dev, "Carrier [%s %d] IO space size is ignored and fixed to 0x%X.\n",
			TVME200_SHORTNAME,
			tvme200->lun, TVME200_IOIDINT_SIZE);

	i = 0;
	while (a16_modifiers[i] != ioid_space->address_modifier) {
		if (a16_modifiers[i] == ENDLIST) {
			dev_err(dev, "Carrier [%s %d] address modifier (0x%X) \
					of ID and IO space is not supported !\n",
					TVME200_SHORTNAME,
					tvme200->lun, ioid_space->address_modifier);
			return -EINVAL;
		}
		i++;
	};

	i = 0;
	while (data_width[i] != ioid_space->data_width) {
		if (data_width[i] == ENDLIST){
			dev_err(dev, "Carrier [%s %d] data width (%d) \
					of ID and IO space is not supported !\n",
					TVME200_SHORTNAME,
					tvme200->lun, ioid_space->data_width);
			return -EINVAL;
		}
		i++;
	};

	if (ioid_space->base_address % TVME200_IOIDINT_SIZE) {
		dev_err(dev, "Carrier [%s %d] base address of ID and IO \
				space must be a boundary equal of the space size (0x%X) !\n",
				TVME200_SHORTNAME,
				tvme200->lun, TVME200_IOIDINT_SIZE);
		return -EINVAL;
	}

	tvme200->info->ioid_mapping.window_num = 0;
	tvme200->info->ioid_mapping.am = ioid_space->address_modifier;
	tvme200->info->ioid_mapping.data_width = ioid_space->data_width;
	tvme200->info->ioid_mapping.vme_addru = 0;
	tvme200->info->ioid_mapping.vme_addrl = ioid_space->base_address;
	tvme200->info->ioid_mapping.sizeu = 0;
	tvme200->info->ioid_mapping.sizel = TVME200_IOIDINT_SIZE;

	return 0;
}

static int set_mem_params(struct tvme200_board *tvme200, struct vme_addr_space *mem_space)
{
	int index = 0;
	int i = 0;
	int *addr_modifier;
	struct device *dev = tvme200->info->dev;

	index = 0;
	while (carrier_mem_size[index] != mem_space->window_size){
		if (carrier_mem_size[index] == ENDLIST){
			dev_err(dev, "Carrier [%s %d] memory size (0x%X) is not supported !\n",
					TVME200_SHORTNAME,
					tvme200->lun, mem_space->window_size);
			return -EINVAL;
		}
		index++;
	};

	if(carrier_mem_size[index] == TVME200_MEM_32M){
		addr_modifier = &a32_modifiers[0];
	}
	else {
		addr_modifier = &a24_modifiers[0];
	}

	i = 0;
	while (addr_modifier[i] != mem_space->address_modifier){
		if (addr_modifier[i] == ENDLIST){
			dev_err(dev, "Carrier [%s %d] address modifier (0x%X) of MEM space is not supported !\n",
					TVME200_SHORTNAME,
					tvme200->lun, mem_space->address_modifier);
			return -EINVAL;
		}
		i++;
	};

	i = 0;
	while (data_width[i] != mem_space->data_width){
		if (data_width[i] == ENDLIST){
			dev_err(dev, "Carrier [%s %d] The data width (%d) of MEM space is not supported !\n",
					TVME200_SHORTNAME,
					tvme200->lun, mem_space->data_width);
			return -EINVAL;
		}
		i++;
	};

	if (mem_space->base_address % carrier_mem_size[index]) {
		dev_err(dev, "Carrier [%s %d] base address of MEM space must be \
				a boundary equal of the space size (0x%X) !\n",
				TVME200_SHORTNAME,
				tvme200->lun, carrier_mem_size[index]);
		return -EINVAL;
	}

	tvme200->info->mem_mapping.window_num = 0;
	tvme200->info->mem_mapping.am = mem_space->address_modifier;
	tvme200->info->mem_mapping.data_width = mem_space->data_width;
	tvme200->info->mem_mapping.vme_addru = 0;
	tvme200->info->mem_mapping.vme_addrl = mem_space->base_address;
	tvme200->info->mem_mapping.sizeu = 0;
	tvme200->info->mem_mapping.sizel = mem_space->window_size;

	return index;
}


/**
 * tvme200_register - Initialisation of the TVME200 board.
 *
 * @carrier		Base structure of the carrier.
 * @dev_specific	Specific device informations.
 *
 */
static int tvme200_register(struct tvme200_board *tvme200)
{
	int res = 0;
	struct tvme200_infos *info = tvme200->info;
	struct device *dev = tvme200->info->dev; 
	int mem_index;
	struct params_vme params;
	int ndev = tvme200->number;
	phys_addr_t ioidint_base;

	memset(&params, 0, sizeof(struct params_vme));
	params.ioid_space.address_modifier = mod_address_ioid[ndev];
	params.ioid_space.base_address = base_address_ioid[ndev];
	params.ioid_space.data_width = data_width_ioid[ndev];
	params.ioid_space.window_size = wind_size_ioid[ndev];

	params.mem_space.address_modifier = mod_address_mem[ndev];
	params.mem_space.base_address = base_address_mem[ndev];
	params.mem_space.data_width = data_width_mem[ndev];
	params.mem_space.window_size = wind_size_mem[ndev];


	res = set_ioid_params(tvme200, &params.ioid_space);
	if (res) {
		goto out_err;
	}

	mem_index = set_mem_params(tvme200, &params.mem_space);
	if (mem_index < 0){
		res = -EINVAL;
		goto out_err;
	}

	res = vme_find_mapping(&info->ioid_mapping, 1);
	if (res) {
		dev_err(dev, "Carrier [%s %d] unable to find window mapping for IO space !\n",
				TVME200_SHORTNAME,
				tvme200->lun);
		goto out_err;
	}

	res = vme_find_mapping(&info->mem_mapping, 1);
	if (res) {
		dev_err(dev, "Carrier [%s %d] unable to find window mapping for MEM space !\n",
				TVME200_SHORTNAME,
				tvme200->lun);
		goto out_release_ioid;
	}

	ioidint_base = info->ioid_mapping.pci_addrl;

	tvme200->mod_mem[IPACK_IO_SPACE] = ioidint_base + TVME200_IO_SPACE_OFF;
	tvme200->mod_mem[IPACK_ID_SPACE] = ioidint_base + TVME200_ID_SPACE_OFF;
	tvme200->mod_mem[IPACK_INT_SPACE] = ioidint_base + TVME200_INT_SPACE_OFF;

	return 0;

out_release_ioid :
	vme_release_mapping(&tvme200->info->ioid_mapping, 1);
out_err :
	return res;
}

/**
 * tvme200_unregister Unregister a TVME200 board.
 *
 * @carrier	Base structure of the board.
 *
 */
static void tvme200_unregister(struct tvme200_board *tvme200)
{
	int error = 0;
	
	error = vme_release_mapping(&tvme200->info->ioid_mapping, 1);
	error = error || vme_release_mapping(&tvme200->info->mem_mapping, 1);

	if (!error)
		dev_info(tvme200->info->dev,"%s: vme mappings released\n", __func__);
	else
		dev_info(tvme200->info->dev,"%s: can't release vme mappings [err %d]\n",
			 __func__,error);
}

static struct tvme200_board *check_slot(struct ipack_device *dev)
{
        struct tvme200_board *tvme200;

        if (dev == NULL)
                return NULL;

        tvme200 = dev_get_drvdata(dev->bus->parent);

        if (tvme200 == NULL) {
                dev_info(&dev->dev, "carrier board not found\n");
                return NULL;
        }

        if (dev->slot >= TVME200_NB_SLOT) {
                dev_info(&dev->dev,
                         "Slot [%d:%d] doesn't exist! Last tvme200 slot is %d.\n",
                         dev->bus->bus_nr, dev->slot, TVME200_NB_SLOT-1);
                return NULL;
        }

        return tvme200;
}

static int tvme200_request_irq(struct ipack_device *dev, irqreturn_t (*handler)(void *), void *arg)
{
	
	int res = 0;
	struct slot_irq *slot_irq;
	struct tvme200_board *tvme200;
	void *irq_reg;

	tvme200 = check_slot(dev);
	if (tvme200 == NULL)
		return -EINVAL;

	if (mutex_lock_interruptible(&tvme200->mutex))
		return -ERESTARTSYS;

	if (tvme200->slots[dev->slot].irq != NULL) {
		dev_err(&dev->dev,
			"Slot [%d:%d] IRQ already registered !\n",
			dev->bus->bus_nr,
			dev->slot);
		res = -EINVAL;
		goto out_unlock;
	}

	slot_irq = kzalloc(sizeof(struct slot_irq), GFP_KERNEL);
	if (slot_irq == NULL) {
		dev_err(&dev->dev,
			"Slot [%d:%d] unable to allocate memory for IRQ !\n",
			dev->bus->bus_nr, dev->slot);
		res = -ENOMEM;
		goto out_unlock;
	}

	/*
	 * WARNING: Setup Interrupt Vector in the IndustryPack device
	 * before an IRQ request.
	 * Read the User Manual of your IndustryPack device to know
	 * where to write the vector in memory.
	 */
	slot_irq->vector = irq[(4 * tvme200->number) + dev->slot];
	slot_irq->handler = (int (*)(void *))(handler); /* FIXME: ugly casting, its ok? */
	slot_irq->arg = arg;
	slot_irq->holder = dev;
	sprintf(slot_irq->name, "ipackdev_%d_%d", tvme200->lun, dev->slot);

	rcu_assign_pointer(tvme200->slots[dev->slot].irq, slot_irq);

	irq_reg = ioremap_nocache(tvme200->mod_mem[IPACK_INT_SPACE] + 
			TVME200_INT_SPACE_INTERVAL * dev->slot, TVME200_ID_SPACE_SIZE);

	iowrite8(slot_irq->vector, irq_reg);
	res = vme_request_irq(slot_irq->vector, slot_irq->handler, slot_irq->arg, slot_irq->name);

	iounmap(irq_reg);
out_unlock:
	mutex_unlock(&tvme200->mutex);
	return res;
}

static int tvme200_free_irq_slot(struct tvme200_board *tvme200, int slot)
{
        struct slot_irq *slot_irq;
	int err;
	struct device *dev = tvme200->info->dev;

        if (tvme200 == NULL)
                return -EINVAL;

        if (mutex_lock_interruptible(&tvme200->mutex))
                return -ERESTARTSYS;
        
        if (tvme200->slots[slot].irq == NULL) {
                mutex_unlock(&tvme200->mutex);
                return -EINVAL;
        }

        slot_irq = tvme200->slots[slot].irq;
	dev_info (dev, "slot %d trying to free irq vector 0x%x\n", slot, slot_irq->vector);
	err = vme_free_irq(slot_irq->vector);
	if (!err)
		dev_info(dev, "slot %d irq vector 0x%x freed\n", slot, slot_irq->vector);
	else
		dev_info(dev, "error: slot %d irq vector 0x%x can not be freed\n", slot, slot_irq->vector);
        kfree(slot_irq);
	slot_irq = NULL;
        mutex_unlock(&tvme200->mutex);
        return 0;
}

static int tvme200_free_irq(struct ipack_device *dev)
{       
        struct tvme200_board *tvme200;
        
        tvme200 = check_slot(dev);

	return tvme200_free_irq_slot(tvme200, dev->slot);
}

static int tvme200_get_clockrate(struct ipack_device *dev)
{
	return 0;
}

static int tvme200_set_clockrate(struct ipack_device *dev, int mherz)
{
	return 0;
}

static int tvme200_get_error(struct ipack_device *dev)
{
	return 0;
}

static int tvme200_get_timeout(struct ipack_device *dev)
{
	return 0;
}

static int tvme200_reset_timeout(struct ipack_device *dev)
{
	return 0;
}

static const struct ipack_bus_ops tvme200_bus_ops = {
        .request_irq =   tvme200_request_irq,
        .free_irq = 	 tvme200_free_irq,
	.get_clockrate = tvme200_get_clockrate,
	.set_clockrate = tvme200_set_clockrate,
	.get_error     = tvme200_get_error,
	.get_timeout   = tvme200_get_timeout,
	.reset_timeout = tvme200_reset_timeout,
};

static int tvme200_install(struct tvme200_board *tvme200, unsigned int n)
{
	int res = 0;
	struct device *dev = tvme200->info->dev;

	dev_info(dev, "Carrier TVME200 lun %d installing...\n", lun[n]);

	tvme200->number = n;
	tvme200->lun	= lun[n];
	tvme200->slots = (struct tvme200_slot*) kzalloc(TVME200_NB_SLOT 
				* sizeof(struct tvme200_slot), GFP_KERNEL);
				
	if (tvme200->slots == NULL) {
		dev_err(dev, "Carrier %d, unable to allocate slots memory !\n", n);
		res = -ENOMEM;
		goto out_err;
	}
	
	
	res = tvme200_register(tvme200);
	if (res){
		dev_err(dev, "Carrier %d, unable to init board !\n", n);
		goto out_free;
	}
	
	mutex_init(&tvme200->mutex);
	dev_info(dev, "Carrier %s lun %d installed.\n", TVME200_SHORTNAME, lun[n]);
	return 0;

out_free :
	kfree(tvme200->slots);
	tvme200->slots = NULL;
out_err :
	return res;
}

static void tvme200_release_device (struct ipack_device *dev)
{
	kfree(dev);
}

static int tvme200_create_mezz_device(struct tvme200_board *tvme200, int i)
{
	enum ipack_space space;
	struct ipack_device *dev =
        	kzalloc(sizeof(struct ipack_device), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->slot = i;
	dev->bus = tvme200->info->ipack_bus;
	dev->release = tvme200_release_device;

	for (space = 0; space < TVME200_SPACE_NUM; space++) {
		dev->region[space].start = 
			tvme200->mod_mem[space]
                     	+ tvme200_space_interval[space] * i;
                 dev->region[space].size = tvme200_space_size[space];
         }
	return ipack_device_register(dev);

}

static void tvme200_uninstall(struct tvme200_board *tvme200)
{
	tvme200_unregister(tvme200);
	
	if (tvme200->slots != NULL)
		kfree(tvme200->slots);
}

static int tvme200_remove(struct device *dev, unsigned int ndev)
{
	struct tvme200_board *tvme200 = &carrier_boards[ndev];

	ipack_bus_unregister(tvme200->info->ipack_bus);
	
	tvme200_uninstall(tvme200);	

	kfree(tvme200->info);
	kfree(tvme200);

	return 0;
}

static int tvme200_probe(struct device *dev, unsigned int ndev)
{
	struct tvme200_board *tvme200 = &carrier_boards[ndev];
	int i;
	int res = 0;

	tvme200->info = (struct tvme200_infos*) kzalloc(sizeof(struct tvme200_infos), GFP_KERNEL);
	if (tvme200->info == NULL) {
		dev_err(dev, "Carrier [%s %d] unable to allocate memory for new carrier !\n",
				TVME200_SHORTNAME,
				ndev);
		res = -ENOMEM;
		goto out_err;
	}

	tvme200->info->dev = dev;

	res = tvme200_install(tvme200, ndev);
	if (res) {
		dev_err(dev, "Error during carrier install !\n");
		goto out_free;
	}

	/* Register the carrier in the industry pack bus driver */
	tvme200->info->ipack_bus = ipack_bus_register(dev,
                                              TVME200_NB_SLOT,
                                              &tvme200_bus_ops);
	if (!tvme200->info->ipack_bus) {
        	dev_err(dev, "error registering the carrier on ipack driver\n");
        	res = -EFAULT;
	        goto out_free;
	}

	dev_set_drvdata(dev, tvme200);
	
	for (i = 0; i < TVME200_NB_SLOT; i++)
        	tvme200_create_mezz_device(tvme200, i);
	
	return 0;

out_free :
	tvme200_uninstall(tvme200);
	kfree(tvme200->info);
	tvme200->info = NULL;
out_err :
	return res;
}

static int tvme200_check_params(void) 
{
	int res = 0;

	if (	(num_lun != num_address_ioid) ||
		(num_lun != num_data_width_ioid) ||
		(num_lun != num_wind_size_mem) ||
		(num_lun != num_base_address_ioid) ||
		(num_lun != num_address_mem) ||
		(num_lun != num_data_width_mem) ||
		(num_lun != num_wind_size_mem) ||
		(num_lun != num_base_address_mem) ) {

			pr_err("%s: The number of parameters doesn't match\n",
				__func__);
			res = -EINVAL;
	}

	if (4 * num_lun  != num_irq) {
			pr_err("%s: The number of IRQ parameters doesn't match\n",
				__func__);
			pr_err("%s: You must specify 4 irq vectors for each carrier." 
				" -1 if slot is empty\n", __func__);
			res = -EINVAL;
	}

	return res;
}

static struct vme_driver tvme200_driver = {
        .probe          = tvme200_probe,
        .remove         = __devexit_p(tvme200_remove),
        .driver         = {
        .name           = KBUILD_MODNAME,
        },
};

static int __init tvme200_init(void)
{
	int error = 0;

	error = tvme200_check_params();
	if (error)
		return -EINVAL;

	carrier_boards = (struct tvme200_board*) 
			kzalloc(num_lun * sizeof(struct tvme200_board), GFP_KERNEL);
	if (carrier_boards == NULL) {
		printk(KERN_ERR PFX "Unable to allocate carrier boards structure !\n");
		return -ENOMEM;
	}

	error = vme_register_driver(&tvme200_driver, num_lun);
	if (error) {
		pr_err("%s: Cannot register vme driver \n", __func__);
		goto vme_error;
	}

	printk(KERN_INFO PFX "Carrier driver loaded.\n");

	return 0;

vme_error:
	kfree(carrier_boards);
	carrier_boards = NULL;
	return error;
}

static void __exit tvme200_exit(void)
{
	vme_unregister_driver(&tvme200_driver);
	
	kfree(carrier_boards);
	carrier_boards = NULL;

	printk(KERN_INFO PFX "Carrier driver unloaded.\n");
}

module_init(tvme200_init);
module_exit(tvme200_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Carrier interface driver PCI-VME");
MODULE_AUTHOR("Nicolas Serafini, EIC2 SA");
MODULE_VERSION("1.1");
