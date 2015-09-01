/*
 * tsi148.c - Low level support for the Tundra TSI148 PCI-VME Bridge Chip
 *
 * Copyright (c) 2009 Sebastien Dugue
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/byteorder.h>

#include "vme_bridge.h"
#include "tsi148.h"

#undef DEBUG_DMA

static struct tsi148_chip *chip;
static struct dma_pool *dma_desc_pool;

static inline void reg_join(u32 high, u32 low, u64 *out)
{
	*out = (unsigned long long)high << 32;
	*out |= low;
}

/*
 * Procfs support
 */

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *tsi148_proc_root;

/**
 * tsi148_proc_pcfs_show() - Dump the PCFS register group
 *
 */
static int tsi148_proc_pcfs_show(struct seq_file *m, void *data)
{
        unsigned int tmp;

        seq_printf(m, "\nPCFS Register Group:\n");

        tmp = ioread32(&chip->pcfs.veni);
        seq_printf(m, "\tpciveni   0x%04x\n", tmp & 0xffff);
        seq_printf(m, "\tpcidevi   0x%04x\n", (tmp>>16) & 0xffff);
        tmp = ioread32(&chip->pcfs.cmmd);
        seq_printf(m, "\tpcicmd    0x%04x\n", tmp & 0xffff);
        seq_printf(m, "\tpcistat   0x%04x\n", (tmp>>16) & 0xffff);
        tmp = ioread32(&chip->pcfs.rev_class);
        seq_printf(m, "\tpcirev    0x%02x\n", tmp & 0xff);
        seq_printf(m, "\tpciclass  0x%06x\n", (tmp >> 8) & 0xffffff);
        tmp = ioread32(&chip->pcfs.clsz);
        seq_printf(m, "\tpciclsz   0x%02x\n", tmp & 0xff);
        seq_printf(m, "\tpcimlat   0x%02x\n", (tmp>>8) & 0xff);
        tmp = ioread32(&chip->pcfs.mbarl);
        seq_printf(m, "\tpcimbarl  0x%08x\n", tmp);
        tmp = ioread32(&chip->pcfs.mbaru);
        seq_printf(m, "\tpcimbaru  0x%08x\n", tmp);
        tmp = ioread32(&chip->pcfs.subv);
        seq_printf(m, "\tpcisubv   0x%04x\n", tmp & 0xffff);
        seq_printf(m, "\tpcisubi   0x%04x\n", (tmp>>16) & 0xffff);
        tmp = ioread32(&chip->pcfs.intl);
        seq_printf(m, "\tpciintl   0x%02x\n", tmp & 0xff);
        seq_printf(m, "\tpciintp   0x%02x\n", (tmp>>8) & 0xff);
        seq_printf(m, "\tpcimngn   0x%02x\n", (tmp>>16) & 0xff);
        seq_printf(m, "\tpcimxla   0x%02x\n", (tmp>>24) & 0xff);
        tmp = ioread32(&chip->pcfs.pcix_cap_id);
        seq_printf(m, "\tpcixcap   0x%02x\n", tmp & 0xff);
        tmp = ioread32(&chip->pcfs.pcix_status);
        seq_printf(m, "\tpcixstat  0x%08x\n", tmp);

        return 0;
}

static int tsi148_proc_pcfs_open(struct inode *inode, struct file *file)
{
        return single_open(file, tsi148_proc_pcfs_show, NULL);
}

static const struct file_operations tsi148_proc_pcfs_ops = {
        .open           = tsi148_proc_pcfs_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};


/**
 * tsi148_proc_lcsr_show() - Dump the LCSR register group
 *
 */
static int tsi148_proc_lcsr_show(struct seq_file *m, void *data)
{
	int i;

	seq_printf(m, "\n");

	/* Display outbound decoders */
	seq_printf(m, "Local Control and Status Register Group (LCSR):\n");
	seq_printf(m, "\nOutbound Translations:\n");
	seq_printf(m, "No. otat         otsau:otsal        oteau:oteal"
		     "         otofu:otofl\n");
	for (i = 0; i < 8; i++) {
		unsigned int otat, otsau, otsal, oteau, oteal, otofu, otofl;

		otat = ioread32be(&chip->lcsr.otrans[i].otat);
		otsau = ioread32be(&chip->lcsr.otrans[i].otsau);
		otsal = ioread32be(&chip->lcsr.otrans[i].otsal);
		oteau = ioread32be(&chip->lcsr.otrans[i].oteau);
		oteal = ioread32be(&chip->lcsr.otrans[i].oteal);
		otofu = ioread32be(&chip->lcsr.otrans[i].otofu);
		otofl = ioread32be(&chip->lcsr.otrans[i].otofl);
		seq_printf(m, "%d:  %08X  %08X:%08X  %08X:%08X:  %08X:%08X\n", 
			i, otat, otsau, otsal, oteau, oteal, otofu, otofl);
	}

	/* Display inbound decoders */
	seq_printf(m, "\nInbound Translations:\n");
	seq_printf(m, "No. itat         itsau:itsal        iteau:iteal"
		     "         itofu:itofl\n");
	for (i = 0; i < 8; i++) {
		unsigned int itat, itsau, itsal, iteau, iteal, itofu, itofl;

		itat = ioread32be(&chip->lcsr.itrans[i].itat);
		itsau = ioread32be(&chip->lcsr.itrans[i].itsau);
		itsal = ioread32be(&chip->lcsr.itrans[i].itsal);
		iteau = ioread32be(&chip->lcsr.itrans[i].iteau);
		iteal = ioread32be(&chip->lcsr.itrans[i].iteal);
		itofu = ioread32be(&chip->lcsr.itrans[i].itofu);
		itofl = ioread32be(&chip->lcsr.itrans[i].itofl);
		seq_printf(m, "%d:  %08X  %08X:%08X  %08X:%08X:  %08X:%08X\n",
			i, itat, itsau, itsal, iteau, iteal, itofu, itofl);
	}

	seq_printf(m, "\nVME Bus Control:\n");
	seq_printf(m, "\tvmctrl  0x%08x\n", ioread32be(&chip->lcsr.vmctrl));
	seq_printf(m, "\tvctrl   0x%08x\n", ioread32be(&chip->lcsr.vctrl));
	seq_printf(m, "\tvstat   0x%08x\n", ioread32be(&chip->lcsr.vstat));

	seq_printf(m, "PCI Status:\n");
	seq_printf(m, "\tpcsr    0x%08x\n", ioread32be(&chip->lcsr.pstat));

	seq_printf(m, "VME Exception Status:\n");
	seq_printf(m, "\tveau:veal 0x%08x:%08x\n",
		     ioread32be(&chip->lcsr.veau),
		     ioread32be(&chip->lcsr.veal));
	seq_printf(m, "\tveat    0x%08x\n", ioread32be(&chip->lcsr.veat));

	seq_printf(m, "PCI Error Status:\n");
	seq_printf(m, "\tedpau:edpal 0x%08x:%08x\n",
		     ioread32be(&chip->lcsr.edpau),
		     ioread32be(&chip->lcsr.edpal));
        seq_printf(m, "\tedpxa   0x%08x\n", ioread32be(&chip->lcsr.edpxa));
        seq_printf(m, "\tedpxs   0x%08x\n", ioread32be(&chip->lcsr.edpxs));
        seq_printf(m, "\tedpat   0x%08x\n", ioread32be(&chip->lcsr.edpat));

	seq_printf(m, "Inbound Translation Location Monitor:\n");
	seq_printf(m, "\tlmbau:lmbal 0x%08x:%08x:\n",
		     ioread32be(&chip->lcsr.lmbau),
		     ioread32be(&chip->lcsr.lmbal));
	seq_printf(m, "\tlmat    0x%08x\n", ioread32be(&chip->lcsr.lmat));

	seq_printf(m, "Local bus Interrupt Control:\n");
	seq_printf(m, "\tinten   0x%08x\n", ioread32be(&chip->lcsr.inten));
	seq_printf(m, "\tinteo   0x%08x\n", ioread32be(&chip->lcsr.inteo));
	seq_printf(m, "\tints    0x%08x\n", ioread32be(&chip->lcsr.ints));
	seq_printf(m, "\tintc    0x%08x\n", ioread32be(&chip->lcsr.intc));
	seq_printf(m, "\tintm1   0x%08x\n", ioread32be(&chip->lcsr.intm1));
	seq_printf(m, "\tintm2   0x%08x\n", ioread32be(&chip->lcsr.intm2));

	seq_printf(m, "VMEbus Interrupt Control:\n");
	seq_printf(m, "\tbcu64   0x%08x\n", ioread32be(&chip->lcsr.bcu));
        seq_printf(m, "\tbcl64   0x%08x\n", ioread32be(&chip->lcsr.bcl));
        seq_printf(m, "\tbpgtr   0x%08x\n", ioread32be(&chip->lcsr.bpgtr));
        seq_printf(m, "\tbpctr   0x%08x\n", ioread32be(&chip->lcsr.bpctr));
	seq_printf(m, "\tvicr    0x%08x\n", ioread32be(&chip->lcsr.vicr));

	seq_printf(m, "RMW Register Group:\n");
	seq_printf(m, "\trmwau  0x%08x\n", ioread32be(&chip->lcsr.rmwau));
	seq_printf(m, "\trmwal  0x%08x\n", ioread32be(&chip->lcsr.rmwal));
	seq_printf(m, "\trmwen  0x%08x\n", ioread32be(&chip->lcsr.rmwen));
	seq_printf(m, "\trmwc   0x%08x\n", ioread32be(&chip->lcsr.rmwc));
	seq_printf(m, "\trmws   0x%08x\n", ioread32be(&chip->lcsr.rmws));

	for(i = 0; i < TSI148_NUM_DMA_CHANNELS; i++){
		seq_printf(m, "DMA Controler %d Registers\n", i);
		seq_printf(m, "\tdctl   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dctl));
		seq_printf(m, "\tdsta   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dsta));
		seq_printf(m, "\tdcsau  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dcsau));
		seq_printf(m, "\tdcsal  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dcsal));
		seq_printf(m, "\tdcdau  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dcdau));
		seq_printf(m, "\tdcdal  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dcdal));
		seq_printf(m, "\tdclau  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dclau));
		seq_printf(m, "\tdclal  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dclal));
		seq_printf(m, "\tdsau   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dsau));
		seq_printf(m, "\tdsal   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dsal));
		seq_printf(m, "\tddau   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.ddau));
		seq_printf(m, "\tddal   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.ddal));
		seq_printf(m, "\tdsat   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dsat));
		seq_printf(m, "\tddat   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.ddat));
		seq_printf(m, "\tdnlau  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dnlau));
		seq_printf(m, "\tdnlal  0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dnlal));
		seq_printf(m, "\tdcnt   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.dcnt));
		seq_printf(m, "\tddbs   0x%08x\n",
			     ioread32be(&chip->lcsr.dma[i].dma_desc.ddbs));
	}
	return 0;
}

static int tsi148_proc_lcsr_open(struct inode *inode, struct file *file)
{
        return single_open(file, tsi148_proc_lcsr_show, NULL);
}

static const struct file_operations tsi148_proc_lcsr_ops = {
        .open           = tsi148_proc_lcsr_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};


/**
 * tsi148_proc_gcsr_show() - Dump the GCSR register group
 *
 */
static int tsi148_proc_gcsr_show(struct seq_file *m, void *data)
{
	int i;
	unsigned int tmp;

	seq_printf(m, "\nGlobal Control and Status Register Group (GCSR):\n");

	tmp = ioread32be(&chip->gcsr.devi);
	seq_printf(m, "\tveni   0x%04x\n", tmp & 0xffff);
	seq_printf(m, "\tdevi   0x%04x\n", (tmp >> 16) & 0xffff);

	tmp = ioread32be(&chip->gcsr.ctrl);
	seq_printf(m, "\tgctrl  0x%04x\n", (tmp >> 16) & 0xffff);
	seq_printf(m, "\tga     0x%02x\n", (tmp >> 8) & 0x3f);
	seq_printf(m, "\trevid  0x%02x\n", tmp & 0xff);

	seq_printf(m, "Semaphores:\n");
	tmp = ioread32be(&chip->gcsr.semaphore[0]);
	seq_printf(m, "\tsem0   0x%02x\n", (tmp >> 24) & 0xff);
	seq_printf(m, "\tsem1   0x%02x\n", (tmp >> 16) & 0xff);
	seq_printf(m, "\tsem2   0x%02x\n", (tmp >> 8) & 0xff);
	seq_printf(m, "\tsem3   0x%02x\n", tmp & 0xff);

	tmp = ioread32be(&chip->gcsr.semaphore[4]);
	seq_printf(m, "\tsem4   0x%02x\n", (tmp >> 24) & 0xff);
	seq_printf(m, "\tsem5   0x%02x\n", (tmp >> 16) & 0xff);
	seq_printf(m, "\tsem6   0x%02x\n", (tmp >> 8) & 0xff);
	seq_printf(m, "\tsem7   0x%02x\n", tmp & 0xff);

	seq_printf(m, "Mailboxes:\n");
	for (i = 0; i <= TSI148_NUM_MAILBOXES; i++){
		seq_printf(m, "\t Mailbox #%d: 0x%08x\n", i,
			     ioread32be(&chip->gcsr.mbox[i]));
	}
	return 0;
}

static int tsi148_proc_gcsr_open(struct inode *inode, struct file *file)
{
        return single_open(file, tsi148_proc_gcsr_show, NULL);
}

static const struct file_operations tsi148_proc_gcsr_ops = {
        .open           = tsi148_proc_gcsr_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

/**
 * tsi148_proc_crcsr_show() - Dump the CRCSR register group
 *
 */
static int tsi148_proc_crcsr_show(struct seq_file *m, void *data)
{
	seq_printf(m, "\nCR/CSR Register Group:\n");
	seq_printf(m, "\tbcr   0x%08x\n", ioread32be(&chip->crcsr.csrbcr));
	seq_printf(m, "\tbsr   0x%08x\n", ioread32be(&chip->crcsr.csrbsr));
        seq_printf(m, "\tbar   0x%08x\n", ioread32be(&chip->crcsr.cbar));
	return 0;
}

static int tsi148_proc_crcsr_open(struct inode *inode, struct file *file)
{
        return single_open(file, tsi148_proc_crcsr_show, NULL);
}

static const struct file_operations tsi148_proc_crcsr_ops = {
        .open           = tsi148_proc_crcsr_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};


/**
 * tsi148_procfs_register() - Create the VME proc tree
 * @vme_root: Root directory of the VME proc tree
 */
void tsi148_procfs_register(struct proc_dir_entry *vme_root)
{
	struct proc_dir_entry *entry;

	tsi148_proc_root = proc_mkdir("tsi148", vme_root);

	entry = proc_create("pcfs", S_IFREG | S_IRUGO, tsi148_proc_root,
			    &tsi148_proc_pcfs_ops);
	if (!entry)
		printk(KERN_WARNING PFX "Failed to create proc pcfs node\n");

	entry = proc_create("lcsr", S_IFREG | S_IRUGO, tsi148_proc_root,
			    &tsi148_proc_lcsr_ops);
	if (!entry)
		printk(KERN_WARNING PFX "Failed to create proc lcsr node\n");

	entry = proc_create("gcsr", S_IFREG | S_IRUGO, tsi148_proc_root,
			    &tsi148_proc_gcsr_ops);
	if (!entry)
		printk(KERN_WARNING PFX "Failed to create proc gcsr node\n");

	entry = proc_create("crcsr", S_IFREG | S_IRUGO, tsi148_proc_root,
			    &tsi148_proc_crcsr_ops);
	if (!entry)
		printk(KERN_WARNING PFX "Failed to create proc crcsr node\n");
}

/**
 * tsi148_procfs_unregister() - Remove the VME proc tree
 *
 */
void tsi148_procfs_unregister(struct proc_dir_entry *vme_root)
{
	remove_proc_entry("pcfs", tsi148_proc_root);
	remove_proc_entry("lcsr", tsi148_proc_root);
	remove_proc_entry("gcsr", tsi148_proc_root);
	remove_proc_entry("crcsr", tsi148_proc_root);
	remove_proc_entry("tsi148", vme_root);
}
#endif /* CONFIG_PROC_FS */

/*
 * PCI and VME bus interrupt handlers
 */


/**
 * tsi148_handle_pci_error() - Handle a PCI bus error reported by the bridge
 *
 *  This handler simply displays a message on the console and clears
 * the error.
 */
void tsi148_handle_pci_error(void)
{
	/* Display raw error information */
	printk(KERN_ERR PFX
	       "PCI error at address: %.8x %.8x - attributes: %.8x\n"
	       "PCI-X attributes: %.8x - PCI-X split completion: %.8x\n",
	       ioread32be(&chip->lcsr.edpau), ioread32be(&chip->lcsr.edpal),
	       ioread32be(&chip->lcsr.edpat),
	       ioread32be(&chip->lcsr.edpxa), ioread32be(&chip->lcsr.edpxs));

	/* Clear error */
	iowrite32be(TSI148_LCSR_EDPAT_EDPCL, &chip->lcsr.edpat);
}

/**
 * tsi148_handle_vme_error() - Handle a VME bus error reported by the bridge
 *
 * Retrieve and report the offending VME address and Address Modifier, clearing
 * the error.
 */
void tsi148_handle_vme_error(struct vme_bus_error *error)
{
	u32 attr;
	u32 addru, addrl;

	/* Store the address */
	addru = ioread32be(&chip->lcsr.veau);
	addrl = ioread32be(&chip->lcsr.veal);
	reg_join(addru, addrl, &error->address);

	/* Get the Address Modifier from the attributes */
	attr = ioread32be(&chip->lcsr.veat);
	error->am = (attr & TSI148_LCSR_VEAT_AM) >> TSI148_LCSR_VEAT_AM_SHIFT;

	/* Display raw error information */
	if (vme_report_bus_errors) {
		printk(KERN_ERR PFX "VME bus error at address: %.8x %.8x - "
			"AM: 0x%x - tsi148 attributes: %.8x\n",
			addru, addrl, error->am, attr);
	}

	/* Overflow detected: not much we can do about it */
	if (attr & TSI148_LCSR_VEAT_VEOF)
		printk(KERN_ERR PFX "VME Bus Exception Overflow Detected\n");

	/* Clear error */
	iowrite32be(TSI148_LCSR_VEAT_VESCL, &chip->lcsr.veat);
}

/**
 * tsi148_generate_interrupt() - Generate an interrupt on the VME bus
 * @level: IRQ level (1-7)
 * @vector: IRQ vector (0-255)
 * @msecs: Timeout for IACK in milliseconds
 *
 *  This function generates an interrupt on the VME bus and waits for IACK
 * for msecs milliseconds.
 *
 *  Returns 0 on success or -ETIME if the timeout expired.
 *
 */
int tsi148_generate_interrupt(int level, int vector, signed long msecs)
{
	unsigned int val;
	signed long timeout = msecs_to_jiffies(msecs);


	if ((level < 1) || (level > 7))
		return -EINVAL;

	if ((vector < 0) || (vector > 255))
		return -EINVAL;

	/* Generate VME IRQ */
	val = ioread32be(&chip->lcsr.vicr) & ~TSI148_LCSR_VICR_STID_M;
	val |= ((level << 8) | vector);
	iowrite32be(val, &chip->lcsr.vicr);

	/* Wait until IACK */
	while (ioread32be(&chip->lcsr.vicr) & TSI148_LCSR_VICR_IRQS) {
		set_current_state(TASK_INTERRUPTIBLE);
		timeout = schedule_timeout(timeout);

		if (timeout == 0)
			break;
	}

	if (ioread32be(&chip->lcsr.vicr) & TSI148_LCSR_VICR_IRQS) {
		/* No IACK received, clear the IRQ */
		val = ioread32be(&chip->lcsr.vicr);

		val &= ~(TSI148_LCSR_VICR_IRQL_M | TSI148_LCSR_VICR_STID_M);
		val |= TSI148_LCSR_VICR_IRQC;
		iowrite32be(val, &chip->lcsr.vicr);

		return -ETIME;
	}


	return 0;
}

/*
 * Utility functions
 */

/**
 * am_to_attr() - Convert a standard VME address modifier to TSI148 attributes
 * @am: Address modifier
 * @addr_size: Address size
 * @transfer_mode: Transfer Mode
 * @user_access: User / Supervisor access
 * @data_access: Data / Program access
 *
 */
static int am_to_attr(enum vme_address_modifier am,
		      unsigned int *addr_size,
		      unsigned int *transfer_mode,
		      unsigned int *user_access,
		      unsigned int *data_access)
{
	switch (am) {
	case VME_A16_USER:
		*addr_size = TSI148_A16;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A16_SUP:
		*addr_size = TSI148_A16;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A24_USER_MBLT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_MBLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A24_USER_DATA_SCT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A24_USER_PRG_SCT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_PROG;
		*user_access = TSI148_USER;
		break;
	case VME_A24_USER_BLT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_BLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A24_SUP_MBLT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_MBLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A24_SUP_DATA_SCT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A24_SUP_PRG_SCT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_PROG;
		*user_access = TSI148_SUPER;
		break;
	case VME_A24_SUP_BLT:
		*addr_size = TSI148_A24;
		*transfer_mode = TSI148_BLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A32_USER_MBLT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_MBLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A32_USER_DATA_SCT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A32_USER_PRG_SCT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_PROG;
		*user_access = TSI148_USER;
		break;
	case VME_A32_USER_BLT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_BLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;
	case VME_A32_SUP_MBLT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_MBLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A32_SUP_DATA_SCT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A32_SUP_PRG_SCT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_PROG;
		*user_access = TSI148_SUPER;
		break;
	case VME_A32_SUP_BLT:
		*addr_size = TSI148_A32;
		*transfer_mode = TSI148_BLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A64_SCT:
		*addr_size = TSI148_A64;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A64_BLT:
		*addr_size = TSI148_A64;
		*transfer_mode = TSI148_BLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_A64_MBLT:
		*addr_size = TSI148_A64;
		*transfer_mode = TSI148_MBLT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_SUPER;
		break;
	case VME_CR_CSR:
		*addr_size = TSI148_CRCSR;
		*transfer_mode = TSI148_SCT;
		*data_access = TSI148_DATA;
		*user_access = TSI148_USER;
		break;

	default:
		return -1;
	}

	return 0;
}

/**
 * attr_to_am() - Convert TSI148 attributes to a standard VME address modifier
 * @addr_size: Address size
 * @transfer_mode: Transfer Mode
 * @user_access: User / Supervisor access
 * @data_access: Data / Program access
 * @am: Address modifier
 *
 */
static void attr_to_am(unsigned int addr_size, unsigned int transfer_mode,
		      unsigned int user_access, unsigned int data_access,
		      int *am)
{

	if (addr_size == TSI148_A16) {
		if (user_access == TSI148_USER)
			*am = VME_A16_USER;
		else
			*am = VME_A16_SUP;
	}
	else if (addr_size == TSI148_A24) {
		if (user_access == TSI148_USER) {
			if (transfer_mode == TSI148_SCT) {
				if (data_access == TSI148_DATA)
					*am = VME_A24_USER_DATA_SCT;
				else
					*am = VME_A24_USER_PRG_SCT;
			} else if (transfer_mode == TSI148_BLT)
				*am = VME_A24_USER_BLT;
			else if (transfer_mode == TSI148_MBLT)
				*am = VME_A24_USER_MBLT;
		} else if (user_access == TSI148_SUPER) {
			if (transfer_mode == TSI148_SCT) {
				if (data_access == TSI148_DATA)
					*am = VME_A24_SUP_DATA_SCT;
				else
					*am = VME_A24_SUP_PRG_SCT;
			} else if (transfer_mode == TSI148_BLT)
				*am = VME_A24_SUP_BLT;
			else if (transfer_mode == TSI148_MBLT)
				*am = VME_A24_SUP_MBLT;
		}
	}
	else if (addr_size == TSI148_A32) {
		if (user_access == TSI148_USER) {
			if (transfer_mode == TSI148_SCT) {
				if (data_access == TSI148_DATA)
					*am = VME_A32_USER_DATA_SCT;
				else
					*am = VME_A32_USER_PRG_SCT;
			} else if (transfer_mode == TSI148_BLT)
				*am = VME_A32_USER_BLT;
			else if (transfer_mode == TSI148_MBLT)
				*am = VME_A32_USER_MBLT;
		} else if (user_access == TSI148_SUPER) {
			if (transfer_mode == TSI148_SCT) {
				if (data_access == TSI148_DATA)
					*am = VME_A32_SUP_DATA_SCT;
				else
					*am = VME_A32_SUP_PRG_SCT;
			} else if (transfer_mode == TSI148_BLT)
				*am = VME_A32_SUP_BLT;
			else if (transfer_mode == TSI148_MBLT)
				*am = VME_A32_SUP_MBLT;
		}
	}
	else if (addr_size == TSI148_A64) {
		if (transfer_mode == TSI148_SCT)
			*am = VME_A64_SCT;
		else if (transfer_mode == TSI148_BLT)
			*am = VME_A64_BLT;
		else if (transfer_mode == TSI148_MBLT)
			*am = VME_A64_MBLT;
	}
	else if (addr_size == TSI148_CRCSR)
		*am = VME_CR_CSR;
}

/*
 * sub64hi() - Calculate the upper 32 bits of the 64 bit difference
 * hi/lo0 - hi/lo1
 */
static int sub64hi(unsigned int lo0, unsigned int hi0,
		   unsigned int lo1, unsigned int hi1)
{
	if (lo0 < lo1)
		return hi0 - hi1 - 1;
	return hi0 - hi1;
}

/*
 * sub64hi() - Calculate the upper 32 bits of the 64 bit sum
 * hi/lo0 + hi/lo1
 */
static int add64hi(unsigned int lo0, unsigned int hi0,
		   unsigned int lo1, unsigned int hi1)
{
	if ((lo0 + lo1) < lo1)
		return hi0 + hi1 + 1;
	return hi0 + hi1;
}

/*
 * TSI148 DMA support
 */

/**
 * tsi148_dma_get_status() - Get the status of a DMA channel
 * @chan: DMA channel descriptor
 *
 **/
int tsi148_dma_get_status(struct dma_channel *chan)
{
	return ioread32be(&chip->lcsr.dma[chan->num].dsta);
}

/**
 * tsi148_dma_busy() - Get the busy status of a DMA channel
 * @chan: DMA channel descriptor
 *
 **/
int tsi148_dma_busy(struct dma_channel *chan)
{
	return tsi148_dma_get_status(chan) & TSI148_LCSR_DSTA_BSY;
}

/**
 * tsi148_dma_done() - Get the done status of a DMA channel
 * @chan: DMA channel descriptor
 *
 */
int tsi148_dma_done(struct dma_channel *chan)
{
	return tsi148_dma_get_status(chan) & TSI148_LCSR_DSTA_DON;
}

/**
 * tsi148_setup_dma_attributes() - Build the DMA attributes word
 * @v2esst_mode: VME 2eSST transfer speed
 * @data_width: VME data width (D16 or D32 only)
 * @am: VME address modifier
 *
 * This function builds the DMA attributes word. All parameters are
 * checked.
 *
 * NOTE: The function relies on the fact that the DSAT and DDAT registers have
 *       the same layout.
 *
 * Returns %EINVAL if any parameter is not consistent.
 */
static int tsi148_setup_dma_attributes(enum vme_2esst_mode v2esst_mode,
				       enum vme_data_width data_width,
				       enum vme_address_modifier am)
{
	unsigned int attrs = 0;
	unsigned int addr_size;
	unsigned int user_access;
	unsigned int data_access;
	unsigned int transfer_mode;

	switch (v2esst_mode) {
	case VME_SST160:
	case VME_SST267:
	case VME_SST320:
		attrs |= ((v2esst_mode << TSI148_LCSR_DSAT_2eSSTM_SHIFT) &
			  TSI148_LCSR_DSAT_2eSSTM_M);
		break;
	default:
		printk(KERN_ERR
		       "%s: invalid v2esst_mode %d\n",
		       __func__, v2esst_mode);
		return -EINVAL;
	}

	switch (data_width) {
	case VME_D16:
		attrs |= ((TSI148_DW_16 << TSI148_LCSR_DSAT_DBW_SHIFT) &
			  TSI148_LCSR_DSAT_DBW_M);
		break;
	case VME_D32:
		attrs |= ((TSI148_DW_32 << TSI148_LCSR_DSAT_DBW_SHIFT) &
			  TSI148_LCSR_DSAT_DBW_M);
		break;
	default:
                printk(KERN_ERR
                       "%s: invalid data_width %d\n",
		       __func__, data_width);
		return -EINVAL;
	}

	if (am_to_attr(am, &addr_size, &transfer_mode, &user_access,
		       &data_access)) {
		printk(KERN_ERR
                       "%s: invalid am %x\n",
		       __func__, am);
		return -EINVAL;
	}

	switch (transfer_mode) {
	case TSI148_SCT:
	case TSI148_BLT:
	case TSI148_MBLT:
	case TSI148_2eVME:
	case TSI148_2eSST:
	case TSI148_2eSSTB:
		attrs |= ((transfer_mode << TSI148_LCSR_DSAT_TM_SHIFT) &
			  TSI148_LCSR_DSAT_TM_M);
		break;
	default:
		printk(KERN_ERR
                       "%s: invalid transfer_mode %d\n",
		       __func__, transfer_mode);
		return -EINVAL;
	}

	switch (user_access) {
	case TSI148_SUPER:
		attrs |= TSI148_LCSR_DSAT_SUP;
		break;
	case TSI148_USER:
		break;
	default:
		printk(KERN_ERR
                       "%s: invalid user_access %d\n",
		       __func__, user_access);
		return -EINVAL;
	}

	switch (data_access) {
	case TSI148_PROG:
		attrs |= TSI148_LCSR_DSAT_PGM;
		break;
	case TSI148_DATA:
		break;
	default:
		printk(KERN_ERR
                       "%s: invalid data_access %d\n",
		       __func__, data_access);
		return -EINVAL;
	}

	switch (addr_size) {
	case TSI148_A16:
	case TSI148_A24:
	case TSI148_A32:
	case TSI148_A64:
	case TSI148_CRCSR:
	case TSI148_USER1:
	case TSI148_USER2:
	case TSI148_USER3:
	case TSI148_USER4:
		attrs |= ((addr_size << TSI148_LCSR_DSAT_AMODE_SHIFT) &
			  TSI148_LCSR_DSAT_AMODE_M);
		break;
	default:
		printk(KERN_ERR
                       "%s: invalid addr_size %d\n",
		       __func__, addr_size);
		return -EINVAL;
	}

	return attrs;
}

/**
 * tsi148_dma_setup_src() - Setup the source attributes for a DMA transfer
 * @desc: DMA transfer descriptor
 *
 */
static int tsi148_dma_setup_src(struct vme_dma *desc)
{
	int rc;

	switch (desc->dir) {
	case VME_DMA_TO_DEVICE: /* src = PCI */
		rc = TSI148_LCSR_DSAT_TYP_PCI;
		break;
	case VME_DMA_FROM_DEVICE: /* src = VME */
		rc = tsi148_setup_dma_attributes(desc->src.v2esst_mode,
						 desc->src.data_width,
						 desc->src.am);

		if (rc >= 0)
			rc |= TSI148_LCSR_DSAT_TYP_VME;

		break;
	default:
		printk(KERN_ERR
                       "%s: invalid direction %d\n",
		       __func__, desc->dir);
		rc = -EINVAL;
	}

	return rc;
}

/**
 * tsi148_dma_setup_dst() - Setup the destination attributes for a DMA transfer
 * @desc: DMA transfer descriptor
 *
 */
static int tsi148_dma_setup_dst(struct vme_dma *desc)
{
	int rc;

	switch (desc->dir) {
	case VME_DMA_TO_DEVICE: /* dst = VME */
		rc = tsi148_setup_dma_attributes(desc->dst.v2esst_mode,
						 desc->dst.data_width,
						 desc->dst.am);

		if (rc >= 0)
			rc |= TSI148_LCSR_DDAT_TYP_VME;

		break;
	case VME_DMA_FROM_DEVICE: /* dst = PCI */
		rc = TSI148_LCSR_DDAT_TYP_PCI;
		break;
	default:
		printk(KERN_ERR
                       "%s: invalid direction %d\n",
		       __func__, desc->dir);
		rc = -EINVAL;
	}

	return rc;
}

/**
 * tsi148_dma_setup_ctl() - Setup the DMA control word
 * @desc: DMA transfer descriptor
 *
 */
static int tsi148_dma_setup_ctl(struct vme_dma *desc)
{
	int rc = 0;

	/* A little bit of checking. Could be done earlier, dunno. */
	if ((desc->ctrl.vme_block_size < VME_DMA_BSIZE_32) ||
	    (desc->ctrl.vme_block_size > VME_DMA_BSIZE_4096))
		return -EINVAL;

	if ((desc->ctrl.vme_backoff_time < VME_DMA_BACKOFF_0) ||
	    (desc->ctrl.vme_backoff_time > VME_DMA_BACKOFF_64))
		return -EINVAL;

	if ((desc->ctrl.pci_block_size < VME_DMA_BSIZE_32) ||
	    (desc->ctrl.pci_block_size > VME_DMA_BSIZE_4096))
		return -EINVAL;

	if ((desc->ctrl.pci_backoff_time < VME_DMA_BACKOFF_0) ||
	    (desc->ctrl.pci_backoff_time > VME_DMA_BACKOFF_64))
		return -EINVAL;

	rc |= (desc->ctrl.vme_block_size << TSI148_LCSR_DCTL_VBKS_SHIFT) &
		TSI148_LCSR_DCTL_VBKS_M;

	rc |= (desc->ctrl.vme_backoff_time << TSI148_LCSR_DCTL_VBOT_SHIFT) &
		TSI148_LCSR_DCTL_VBOT_M;

	rc |= (desc->ctrl.pci_block_size << TSI148_LCSR_DCTL_PBKS_SHIFT) &
		TSI148_LCSR_DCTL_PBKS_M;

	rc |= (desc->ctrl.pci_backoff_time << TSI148_LCSR_DCTL_PBOT_SHIFT) &
		TSI148_LCSR_DCTL_PBOT_M;

	return rc;
}

/**
 * tsi148_dma_free_chain() - Free all the linked HW DMA descriptors of a channel
 * @chan: DMA channel
 *
 */
void tsi148_dma_free_chain(struct dma_channel *chan)
{
	struct hw_desc_entry *hw_desc;
	struct hw_desc_entry *tmp;
	int count = 1;

	list_for_each_entry_safe(hw_desc, tmp, &chan->hw_desc_list, list) {
		pci_pool_free(dma_desc_pool, hw_desc->va, hw_desc->phys);
		list_del(&hw_desc->list);
		kfree(hw_desc);
		count++;
	}
}

static inline int get_vmeaddr(struct vme_dma *desc, unsigned int *vme_addr)
{
	switch (desc->dir) {
	case VME_DMA_TO_DEVICE: /* src = PCI - dst = VME */
		*vme_addr = desc->dst.addrl;
		return 0;
	case VME_DMA_FROM_DEVICE: /* src = VME - dst = PCI */
		*vme_addr = desc->src.addrl;
		return 0;
	default:
		return -EINVAL;
	}
}

static int
hwdesc_init(struct dma_channel *chan, dma_addr_t *phys,
	    struct tsi148_dma_desc **virt, struct hw_desc_entry **hw_desc)
{
	struct hw_desc_entry *entry;

	/* Allocate a HW DMA descriptor from the pool */
	*virt = pci_pool_alloc(dma_desc_pool, GFP_KERNEL, phys);
	if (!*virt)
		return -ENOMEM;

	/* keep the virt. and phys. addresses of the descriptor in a list */
	*hw_desc = kmalloc(sizeof(struct hw_desc_entry), GFP_KERNEL);
	if (!*hw_desc) {
		pci_pool_free(dma_desc_pool, *virt, *phys);
		return -ENOMEM;
	}

	entry = *hw_desc;
	entry->va = *virt;
	entry->phys = *phys;
	list_add_tail(&entry->list, &chan->hw_desc_list);

	return 0;
}

/*
 * Setup the hardware descriptors for the link list.
 * Beware that the fields have to be setup in big endian mode.
 */
static int
tsi148_fill_dma_desc(struct dma_channel *chan, struct tsi148_dma_desc *tsi,
		unsigned int vme_addr, dma_addr_t dma_addr, unsigned int len,
		unsigned int dsat, unsigned int ddat)
{
	struct vme_dma *desc = &chan->desc;

	/* Setup the source and destination addresses */
	tsi->dsau = 0;
	tsi->ddau = 0;

	switch (desc->dir) {
	case VME_DMA_TO_DEVICE: /* src = PCI - dst = VME */
		tsi->dsal = cpu_to_be32(dma_addr);
		tsi->ddal = cpu_to_be32(vme_addr);
		tsi->ddbs = cpu_to_be32(desc->dst.bcast_select);
		break;

	case VME_DMA_FROM_DEVICE: /* src = VME - dst = PCI */
		tsi->dsal = cpu_to_be32(vme_addr);
		tsi->ddal = cpu_to_be32(dma_addr);
		tsi->ddbs = 0;
		break;

	default:
		return -EINVAL;
	}

	tsi->dcnt = cpu_to_be32(len);
	tsi->dsat = cpu_to_be32(dsat);
	tsi->ddat = cpu_to_be32(ddat);

	/* By default behave as if we were at the end of the list */
	tsi->dnlau = 0;
	tsi->dnlal = cpu_to_be32(TSI148_LCSR_DNLAL_LLA);

	return 0;
}

#ifdef DEBUG_DMA
static void tsi148_dma_debug_info(struct tsi148_dma_desc *tsi, int i, int j)
{
	printk(KERN_DEBUG PFX "descriptor %d-%d @%p\n", i, j, tsi);
	printk(KERN_DEBUG PFX "  src : %08x:%08x  %08x\n",
		be32_to_cpu(tsi->dsau), be32_to_cpu(tsi->dsal),
		be32_to_cpu(tsi->dsat));
	printk(KERN_DEBUG PFX "  dst : %08x:%08x  %08x\n",
		be32_to_cpu(tsi->ddau), be32_to_cpu(tsi->ddal),
		be32_to_cpu(tsi->ddat));
	printk(KERN_DEBUG PFX "  cnt : %08x\n",	be32_to_cpu(tsi->dcnt));
	printk(KERN_DEBUG PFX "  nxt : %08x:%08x\n",
		be32_to_cpu(tsi->dnlau), be32_to_cpu(tsi->dnlal));
}
#else
static void tsi148_dma_debug_info(struct tsi148_dma_desc *tsi, int i, int j)
{
}
#endif

/*
 * verbatim from the VME64 standard:
 * 2.3.7 Block Transfer Capabilities, p. 42.
 *	RULE 2.12a: D08(EO), D16, D32 and MD32 block transfer cycles
 *	  (BLT) *MUST NOT* cross any 256 byte boundary.
 * 	RULE 2.78: MBLT cycles *MUST NOT* cross any 2048 byte boundary.
 */
static inline int __tsi148_get_bshift(int am)
{
	switch (am) {
	case VME_A64_MBLT:
	case VME_A32_USER_MBLT:
	case VME_A32_SUP_MBLT:
	case VME_A24_USER_MBLT:
	case VME_A24_SUP_MBLT:
		return 11;
	case VME_A64_BLT:
	case VME_A32_USER_BLT:
	case VME_A32_SUP_BLT:
	case VME_A40_BLT:
	case VME_A24_USER_BLT:
	case VME_A24_SUP_BLT:
		return 8;
	default:
		return 0;
	}
}

static int tsi148_get_bshift(struct dma_channel *chan)
{
	struct vme_dma *desc = &chan->desc;
	int am;

	if (desc->dir == VME_DMA_FROM_DEVICE)
		am = desc->src.am;
	else
		am = desc->dst.am;
	return __tsi148_get_bshift(am);
}

/* Note: block sizes are always powers of 2 */
static inline unsigned long tsi148_get_bsize(int bshift)
{
	return 1 << bshift;
}

static inline unsigned long tsi148_get_bmask(unsigned long bsize)
{
	return bsize - 1;
}

/*
 * Add a certain chunk of data to the TSI DMA linked list.
 * Note that this function deals with physical addresses on the
 * host CPU and VME addresses on the VME bus.
 */
static int
tsi148_dma_link_add(struct dma_channel *chan, struct tsi148_dma_desc **virt,
		unsigned int vme_addr, dma_addr_t dma_addr, unsigned int size,
		int numpages, int index, int bshift, unsigned int dsat,
		unsigned int ddat)

{
	struct hw_desc_entry *hw_desc = NULL;
	struct tsi148_dma_desc *curr;
	struct tsi148_dma_desc *next = NULL;
	struct vme_dma *desc = &chan->desc;
	dma_addr_t phys_next;
	dma_addr_t dma;
	dma_addr_t dma_end;
	unsigned int vme;
	unsigned int vme_end;
	unsigned int len;
	int rc;
	int i = 0;

	/*
	 * The function expects **virt to be already initialised.
	 * When calling this function in a loop, it will always set virt
	 * to point to the next link in the chain, or to NULL when the
	 * current link is the last in the chain.
	 */
	BUG_ON(virt == NULL || *virt == NULL);
	curr = *virt;

	vme = vme_addr;
	vme_end = vme_addr + size;

	dma = dma_addr;
	dma_end = dma_addr + size;

	len = size;

	while (vme < vme_end && dma < dma_end) {

		/* calculate the length up to the next block boundary */
		if (bshift) {
			unsigned long bsize = tsi148_get_bsize(bshift);
			unsigned long bmask = tsi148_get_bmask(bsize);
			unsigned int unaligned = vme & bmask;

			len = bsize - unaligned;
		}

		/* check the VME block won't overflow */
		if (dma + len > dma_end)
			len = dma_end - dma;

		tsi148_fill_dma_desc(chan, curr, vme, dma, len, dsat, ddat);

		/* increment the VME address unless novmeinc is set */
		if (!desc->novmeinc)
			vme += len;
		dma += len;

		/* chain consecutive links together */
		if (index < numpages - 1 || dma < dma_end) {
			rc = hwdesc_init(chan, &phys_next, &next, &hw_desc);
			if (rc)
				return rc;

			curr->dnlau = 0;
			curr->dnlal = cpu_to_be32(phys_next &
						TSI148_LCSR_DNLAL_DNLAL_M);
		}

		tsi148_dma_debug_info(curr, index, i);

		curr = next;
		i++;
	}

	*virt = next;
	return 0;
}

/**
 * tsi148_dma_setup_chain() - Setup the linked list of TSI148 DMA descriptors
 * @chan: DMA channel descriptor
 * @dsat: DMA source attributes
 * @ddat: DMA destination attributes
 *
 */
static int tsi148_dma_setup_chain(struct dma_channel *chan,
				  unsigned int dsat, unsigned int ddat)
{
	int i;
	int rc;
	struct scatterlist *sg;
	struct vme_dma *desc = &chan->desc;
	dma_addr_t phys_start;
	struct tsi148_dma_desc *curr = NULL;
	struct hw_desc_entry *hw_desc = NULL;
	unsigned int vme_addr = 0;
	dma_addr_t dma_addr;
	unsigned int len;
	int bshift = tsi148_get_bshift(chan);

	rc = hwdesc_init(chan, &phys_start, &curr, &hw_desc);
	if (rc)
		return rc;

	rc = get_vmeaddr(desc, &vme_addr);
	if (rc)
		goto out_free;

	for_each_sg(chan->sgl, sg, chan->sg_mapped, i) {
		dma_addr = sg_dma_address(sg);
		len = sg_dma_len(sg);

		rc = tsi148_dma_link_add(chan, &curr, vme_addr, dma_addr, len,
					chan->sg_mapped, i, bshift, dsat, ddat);
		if (rc)
			goto out_free;
		/* For non incrementing DMA, reset the VME address */
		if (!desc->novmeinc)
			vme_addr += len;
	}

	/* Now program the DMA registers with the first entry in the list */
	iowrite32be(0, &chip->lcsr.dma[chan->num].dma_desc.dnlau);
	iowrite32be(phys_start & TSI148_LCSR_DNLAL_DNLAL_M,
		    &chip->lcsr.dma[chan->num].dma_desc.dnlal);

	return 0;

out_free:
	tsi148_dma_free_chain(chan);


	return rc;
}

/**
 * tsi148_dma_setup() - Setup a TSI148 DMA channel for a transfer
 * @chan: DMA channel descriptor
 *
 */
int tsi148_dma_setup(struct dma_channel *chan)
{
	int rc;
	struct vme_dma *desc = &chan->desc;
	unsigned int dsat;
	unsigned int ddat;
	unsigned int dctl;

	/* Setup DMA source attributes */
	if ((rc = tsi148_dma_setup_src(desc)) < 0) {
		printk(KERN_ERR "%s: src setup failed\n", __func__);
		return rc;
	}

	dsat = rc;

	/* Setup DMA destination attributes */
	if ((rc = tsi148_dma_setup_dst(desc)) < 0) {
		printk(KERN_ERR "%s: dst setup failed\n", __func__);
		return rc;
	}

	ddat = rc;

	/* Setup DMA control */
	if ((rc = tsi148_dma_setup_ctl(desc)) < 0) {
		printk(KERN_ERR "%s: ctl setup failed\n", __func__);
		return rc;
	}

	dctl = rc;

	/* always use the scatter-gather list */
	chan->chained = 1;
	dctl &= ~TSI148_LCSR_DCTL_MOD;
	iowrite32be(dctl, &chip->lcsr.dma[chan->num].dctl);
	rc = tsi148_dma_setup_chain(chan, dsat, ddat);

	return rc;
}

/**
 * tsi148_dma_start() - Start DMA transfer on a channel
 * @chan: DMA channel descriptor
 *
 */
void tsi148_dma_start(struct dma_channel *chan)
{
	unsigned int dctl;

	dctl = ioread32be(&chip->lcsr.dma[chan->num].dctl);
	dctl |= TSI148_LCSR_DCTL_DGO;

	iowrite32be(dctl, &chip->lcsr.dma[chan->num].dctl);
}

/**
 * tsi148_dma_abort() - Abort a DMA transfer
 * @chan: DMA channel descriptor
 *
 */
void tsi148_dma_abort(struct dma_channel *chan)
{
	unsigned int dctl;

	dctl = ioread32be(&chip->lcsr.dma[chan->num].dctl);
	dctl |= TSI148_LCSR_DCTL_ABT;

	iowrite32be(dctl, &chip->lcsr.dma[chan->num].dctl);
}

/**
 * tsi148_dma_release() - Release resources used for a DMA transfer
 * @chan: DMA channel descriptor
 *
 *  Only chained transfer are allocating memory. For direct transfer, there
 * is nothig to free.
 */
void tsi148_dma_release(struct dma_channel *chan)
{
	if (chan->chained)
		tsi148_dma_free_chain(chan);
}

void tsi148_dma_exit(void)
{
	pci_pool_destroy(dma_desc_pool);
}

int tsi148_dma_init(void)
{
	/*
	 * Create the DMA chained descriptor pool.
	 * Those descriptors must be 64-bit aligned as specified in the
	 * TSI148 User Manual. Also do not allow descriptors to cross a
	 * page boundary as the 2 pages may not be contiguous.
	 */
	dma_desc_pool = pci_pool_create("vme_dma_desc_pool", vme_bridge->pdev,
					sizeof(struct tsi148_dma_desc),
					8, 4096);

	if (dma_desc_pool == NULL) {
		printk(KERN_WARNING PFX "Failed to allocate DMA pool\n");
		return -ENOMEM;
	}

	return 0;

}

/*
 * TSI148 window support
 */

/**
 * tsi148_setup_window_attributes() - Build the window attributes word
 * @v2esst_mode: VME 2eSST transfer speed
 * @data_width: VME data width (D16 or D32 only)
 * @am: VME address modifier
 *
 * This function builds the window attributes word. All parameters are
 * checked.
 *
 * Returns %EINVAL if any parameter is not consistent.
 */
static int tsi148_setup_window_attributes(enum vme_2esst_mode v2esst_mode,
					  enum vme_data_width data_width,
					  enum vme_address_modifier am)
{
	unsigned int attrs = 0;
	unsigned int addr_size;
	unsigned int user_access;
	unsigned int data_access;
	unsigned int transfer_mode;

	switch (v2esst_mode) {
	case VME_SST160:
	case VME_SST267:
	case VME_SST320:
		attrs |= ((v2esst_mode << TSI148_LCSR_OTAT_2eSSTM_SHIFT) &
			  TSI148_LCSR_OTAT_2eSSTM_M);
		break;
	default:
		return -EINVAL;
	}

	switch (data_width) {
	case VME_D16:
		attrs |= ((TSI148_DW_16 << TSI148_LCSR_OTAT_DBW_SHIFT) &
			  TSI148_LCSR_OTAT_DBW_M);
		break;
	case VME_D32:
		attrs |= ((TSI148_DW_32 << TSI148_LCSR_OTAT_DBW_SHIFT) &
			  TSI148_LCSR_OTAT_DBW_M);
		break;
	default:
		return -EINVAL;
	}

	if (am_to_attr(am, &addr_size, &transfer_mode, &user_access,
		       &data_access))
		return -EINVAL;

	switch (transfer_mode) {
	case TSI148_SCT:
	case TSI148_BLT:
	case TSI148_MBLT:
	case TSI148_2eVME:
	case TSI148_2eSST:
	case TSI148_2eSSTB:
		attrs |= ((transfer_mode << TSI148_LCSR_OTAT_TM_SHIFT) &
			  TSI148_LCSR_OTAT_TM_M);
		break;
	default:
		return -EINVAL;
	}

	switch (user_access) {
	case TSI148_SUPER:
		attrs |= TSI148_LCSR_OTAT_SUP;
		break;
	case TSI148_USER:
		break;
	default:
		return -EINVAL;
	}

	switch (data_access) {
	case TSI148_PROG:
		attrs |= TSI148_LCSR_OTAT_PGM;
		break;
	case TSI148_DATA:
		break;
	default:
		return -EINVAL;
	}

	switch (addr_size) {
	case TSI148_A16:
	case TSI148_A24:
	case TSI148_A32:
	case TSI148_A64:
	case TSI148_CRCSR:
	case TSI148_USER1:
	case TSI148_USER2:
	case TSI148_USER3:
	case TSI148_USER4:
		attrs |= ((addr_size << TSI148_LCSR_OTAT_AMODE_SHIFT) &
			  TSI148_LCSR_OTAT_AMODE_M);
		break;
	default:
		return -EINVAL;
	}

	return attrs;
}

/**
 * tsi148_get_window_attr() - Get a hardware window attributes
 * @desc: Descriptor of the window (only the window number is used)
 *
 */
void tsi148_get_window_attr(struct vme_mapping *desc)
{
	int window_num = desc->window_num;
	unsigned int transfer_mode;
	unsigned int user_access;
	unsigned int data_access;
	unsigned int addr_size;
	int am = -1;
	struct tsi148_otrans *trans = &chip->lcsr.otrans[window_num];
	unsigned int otat;
	unsigned int otsau, otsal;
	unsigned int oteau, oteal;
	unsigned int otofu, otofl;

	/* Get window Control & Bus attributes register */
	otat = ioread32be(&trans->otat);

	if (otat & TSI148_LCSR_OTAT_EN)
		desc->window_enabled = 1;

	if (!(otat & TSI148_LCSR_OTAT_MRPFD))
		desc->read_prefetch_enabled = 1;

	desc->read_prefetch_size = (otat & TSI148_LCSR_OTAT_PFS_M) >>
		TSI148_LCSR_OTAT_PFS_SHIFT;

	desc->v2esst_mode = (otat & TSI148_LCSR_OTAT_2eSSTM_M) >>
		TSI148_LCSR_OTAT_2eSSTM_SHIFT;

	switch ((otat & TSI148_LCSR_OTAT_DBW_M) >> TSI148_LCSR_OTAT_DBW_SHIFT) {
	case TSI148_LCSR_OTAT_DBW_16:
		desc->data_width = VME_D16;
		break;
	case TSI148_LCSR_OTAT_DBW_32:
		desc->data_width = VME_D32;
		break;
	}

	desc->bcast_select = ioread32be(&chip->lcsr.otrans[window_num].otbs);

	/*
	 *  The VME address modifiers is encoded into the 4 following registers.
	 * Here we then have to map that back to a single value - This is
	 * coming out real ugly (sigh).
	 */
	addr_size = (otat & TSI148_LCSR_OTAT_AMODE_M) >>
		TSI148_LCSR_OTAT_AMODE_SHIFT;

	transfer_mode = (otat & TSI148_LCSR_OTAT_TM_M) >>
		TSI148_LCSR_OTAT_TM_SHIFT;

	user_access = (otat & TSI148_LCSR_OTAT_SUP) ?
		TSI148_SUPER : TSI148_USER;

	data_access = (otat & TSI148_LCSR_OTAT_PGM) ?
		TSI148_PROG : TSI148_DATA;

	attr_to_am(addr_size, transfer_mode, user_access, data_access, &am);

	if (am == -1)
		printk(KERN_WARNING PFX
		       "%s - unsupported AM:\n"
		       "\taddr size: %x TM: %x usr/sup: %d data/prg:%d\n",
		       __func__, addr_size, transfer_mode, user_access,
		       data_access);

	desc->am = am;

	/* Get window mappings */
	otsal = ioread32be(&chip->lcsr.otrans[window_num].otsal);
	otsau = ioread32be(&chip->lcsr.otrans[window_num].otsau);
	oteal = ioread32be(&chip->lcsr.otrans[window_num].oteal);
	oteau = ioread32be(&chip->lcsr.otrans[window_num].oteau);
	otofl = ioread32be(&chip->lcsr.otrans[window_num].otofl);
	otofu = ioread32be(&chip->lcsr.otrans[window_num].otofu);


	desc->pci_addrl = otsal;
	desc->pci_addru = otsau;

	desc->sizel = oteal - otsal;
	desc->sizeu = sub64hi(oteal, oteau, otsal, otsau);

	desc->vme_addrl = otofl + otsal;

	if (addr_size == TSI148_A64)
		desc->vme_addru = add64hi(otofl, otofu, otsal, otsau);
	else
		desc->vme_addru = 0;
}

/**
 * tsi148_create_window() - Create a PCI-VME window
 * @desc: Window descriptor
 *
 * Setup the TSI148 outbound window registers and enable the window for
 * use.
 *
 * Returns 0 on success, %EINVAL in case of wrong parameter.
 */
int tsi148_create_window(struct vme_mapping *desc)
{
	int window_num = desc->window_num;
	int rc;
	unsigned int addr;
	unsigned int size;
	struct tsi148_otrans *trans = &chip->lcsr.otrans[window_num];
	unsigned int otat = 0;
	unsigned int oteal, oteau;
	unsigned int otofl, otofu;

	/*
	 * Do some sanity checking on the window descriptor.
	 * PCI address, VME address and window size should be aligned
	 * on 64k boudaries. So round down the VME address and round up the
	 * VME size to 64K boundaries.
	 * The PCI address should already have been rounded.
	 */
	addr = desc->vme_addrl;
	size = desc->sizel;

	if (addr & 0xffff)
		addr = desc->vme_addrl & ~0xffff;

	if (size & 0xffff)
		size = (desc->vme_addrl + desc->sizel + 0x10000) & ~0xffff;

	desc->vme_addrl = addr;
	desc->sizel = size;

	if (desc->pci_addrl & 0xffff)
		return -EINVAL;

	/* Setup the window attributes */
	rc = tsi148_setup_window_attributes(desc->v2esst_mode,
					    desc->data_width,
					    desc->am);

	if (rc < 0)
		return rc;

	otat = (unsigned int)rc;

	/* Enable the window */
	otat |= TSI148_LCSR_OTAT_EN;

	if (desc->read_prefetch_enabled) {
		/* clear Read prefetch disable bit */
		otat &= ~TSI148_LCSR_OTAT_MRPFD;
		otat |= ((desc->read_prefetch_size <<
			  TSI148_LCSR_OTAT_PFS_SHIFT) & TSI148_LCSR_OTAT_PFS_M);
	} else {
		/* Set Read prefetch disable bit */
		otat |= TSI148_LCSR_OTAT_MRPFD;
	}

	/* Setup the VME 2eSST broadcast select */
	iowrite32be(desc->bcast_select & TSI148_LCSR_OTBS_M,
		    &trans->otbs);

	/* Setup the PCI side start address */
	iowrite32be(desc->pci_addrl & TSI148_LCSR_OTSAL_M,
		    &trans->otsal);
	iowrite32be(desc->pci_addru, &trans->otsau);

	/*
	 * Setup the PCI side end address
	 * Note that 'oteal' is formed by the upper 2 bytes common to the
	 * addresses in the last 64K chunk of the window.
	 */
	oteal = (desc->pci_addrl + desc->sizel - 1) & ~0xffff;
	oteau = add64hi(desc->pci_addrl, desc->pci_addru,
			desc->sizel, desc->sizeu);
	iowrite32be(oteal, &trans->oteal);
	iowrite32be(oteau, &trans->oteau);

	/* Setup the VME offset */
	otofl = desc->vme_addrl - desc->pci_addrl;
	otofu = sub64hi(desc->vme_addrl, desc->vme_addru,
			desc->pci_addrl, desc->pci_addru);
	iowrite32be(otofl, &trans->otofl);
	iowrite32be(otofu, &trans->otofu);

	/*
	 * Finally, write the window attributes, also enabling the
	 * window
	 */
	iowrite32be(otat, &trans->otat);
	desc->window_enabled = 1;

	return 0;
}

/**
 * tsi148_remove_window() - Disable a PCI-VME window
 * @desc: Window descriptor (only the window number is used)
 *
 * Disable the PCI-VME window specified in the descriptor.
 */
void tsi148_remove_window(struct vme_mapping *desc)
{
	int window_num = desc->window_num;
	struct tsi148_otrans *trans = &chip->lcsr.otrans[window_num];

	/* Clear the attribute register thus disabling the window */
	iowrite32be(0, &trans->otat);

	/* Wipe the window registers */
	iowrite32be(0, &trans->otsal);
	iowrite32be(0, &trans->otsau);
	iowrite32be(0, &trans->oteal);
	iowrite32be(0, &trans->oteau);
	iowrite32be(0, &trans->otofl);
	iowrite32be(0, &trans->otofu);
	iowrite32be(0, &trans->otbs);

	desc->window_enabled = 0;
}

/**
 * am_to_crgattrs() - Convert address modifier to CRG Attributes
 * @am: Address modifier
 */
static int am_to_crgattrs(enum vme_address_modifier am)
{
	unsigned int attrs = 0;
	unsigned int addr_size;
	unsigned int user_access;
	unsigned int data_access;
	unsigned int transfer_mode;

	if (am_to_attr(am, &addr_size, &transfer_mode, &user_access,
		       &data_access))
		return -EINVAL;

	switch (addr_size) {
	case TSI148_A16:
		attrs |= TSI148_LCSR_CRGAT_AS_16;
		break;
	case TSI148_A24:
		attrs |= TSI148_LCSR_CRGAT_AS_24;
		break;
	case TSI148_A32:
		attrs |= TSI148_LCSR_CRGAT_AS_32;
		break;
	case TSI148_A64:
		attrs |= TSI148_LCSR_CRGAT_AS_64;
		break;
	default:
		return -EINVAL;
	}

	switch (data_access) {
	case TSI148_PROG:
		attrs |= TSI148_LCSR_CRGAT_PGM;
		break;
	case TSI148_DATA:
		attrs |= TSI148_LCSR_CRGAT_DATA;
		break;
	default:
		return -EINVAL;
	}

	switch (user_access) {
	case TSI148_SUPER:
		attrs |= TSI148_LCSR_CRGAT_SUPR;
		break;
	case TSI148_USER:
		attrs |= TSI148_LCSR_CRGAT_NPRIV;
		break;
	default:
		return -EINVAL;
	}

	return attrs;
}

/**
 * tsi148_setup_crg() - Setup the CRG inbound mapping
 * @vme_base: VME base address for the CRG mapping
 * @am: Address modifier for the mapping
 */
int tsi148_setup_crg(unsigned int vme_base,
			       enum vme_address_modifier am)
{
	int attrs;

	iowrite32be(0, &chip->lcsr.cbau);
	iowrite32be(vme_base, &chip->lcsr.cbal);

	attrs = am_to_crgattrs(am);

	if (attrs < 0) {
		iowrite32be(0, &chip->lcsr.cbal);
		return -1;
	}

	attrs |= TSI148_LCSR_CRGAT_EN;

	iowrite32be(attrs, &chip->lcsr.crgat);

	return 0;
}

/**
 * tsi148_disable_crg() - Disable the CRG VME mapping
 *
 */
void tsi148_disable_crg(struct tsi148_chip *regs)
{
	iowrite32be(0, &regs->lcsr.crgat);
	iowrite32be(0, &regs->lcsr.cbau);
	iowrite32be(0, &regs->lcsr.cbal);
}


/**
 * tsi148_quiesce() - Shutdown the TSI148 chip
 *
 * Put VME bridge in quiescent state.  Disable all decoders,
 * clear all interrupts.
 */
void tsi148_quiesce(struct tsi148_chip *regs)
{
	int i;
	unsigned int val;

	/* Shutdown all inbound and outbound windows. */
	for (i = 0; i < TSI148_NUM_OUT_WINDOWS; i++) {
		iowrite32be(0, &regs->lcsr.itrans[i].itat);
		iowrite32be(0, &regs->lcsr.otrans[i].otat);
	}

	for (i= 0; i < TSI148_NUM_DMA_CHANNELS; i++) {
		iowrite32be(0, &regs->lcsr.dma[i].dma_desc.dnlau);
		iowrite32be(0, &regs->lcsr.dma[i].dma_desc.dnlal);
		iowrite32be(0, &regs->lcsr.dma[i].dma_desc.dcnt);
	}

	/* Shutdown Location monitor. */
	iowrite32be(0, &regs->lcsr.lmat);

	/* Shutdown CRG map. */
	tsi148_disable_crg(regs);

	/* Clear error status. */
	iowrite32be(TSI148_LCSR_EDPAT_EDPCL, &regs->lcsr.edpat);
	iowrite32be(TSI148_LCSR_VEAT_VESCL, &regs->lcsr.veat);
	iowrite32be(TSI148_LCSR_PCSR_SRTO | TSI148_LCSR_PCSR_SRTE |
		    TSI148_LCSR_PCSR_DTTE | TSI148_LCSR_PCSR_MRCE,
		    &regs->lcsr.pstat);

	/* Clear VIRQ interrupt (if any) */
	iowrite32be(TSI148_LCSR_VICR_IRQC, &regs->lcsr.vicr);

	/* Disable and clear all interrupts. */
	iowrite32be(0, &regs->lcsr.inteo);
	iowrite32be(0xffffffff, &regs->lcsr.intc);
	iowrite32be(0xffffffff, &regs->lcsr.inten);

	/* Map all Interrupts to PCI INTA */
	iowrite32be(0, &regs->lcsr.intm1);
	iowrite32be(0, &regs->lcsr.intm2);

	/*
	 * Set bus master timeout
	 * The timeout overrides the DMA block size if set too low.
	 * Enable release on request so that there will not be a re-arbitration
	 * for each transfer.
	 */
	val = ioread32be(&regs->lcsr.vmctrl);
	val &= ~(TSI148_LCSR_VMCTRL_VTON_M | TSI148_LCSR_VMCTRL_VREL_M);
	val |= (TSI148_LCSR_VMCTRL_VTON_512 | TSI148_LCSR_VMCTRL_VREL_T_D_R);
	iowrite32be(val, &regs->lcsr.vmctrl);
}

/*
 *
 */

/**
 * tsi148_init() - Chip initialization
 * @regs: Chip registers base address
 *
 */
void tsi148_init(struct tsi148_chip *regs)
{
	unsigned int vme_stat_reg;

	/* Clear board fail, and power-up reset */
	vme_stat_reg = ioread32be(&regs->lcsr.vstat);
	vme_stat_reg &= ~TSI148_LCSR_VSTAT_BDFAIL;
	vme_stat_reg |= TSI148_LCSR_VSTAT_CPURST;
	iowrite32be(vme_stat_reg, &regs->lcsr.vstat);

	/* Store the chip base address */
	chip = regs;
}
