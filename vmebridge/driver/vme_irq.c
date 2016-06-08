/*
 * vme_irq.c - PCI-VME bridge interrupt management
 *
 * Copyright (c) 2009 Sebastien Dugue
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

/*
 *  This file provides the PCI-VME bridge interrupt management code.
 */

#include <linux/interrupt.h>
#include <linux/irqdomain.h>

#include "tsi148.h"
#include "vme_bridge.h"


unsigned int vme_interrupts_enabled;

struct vme_irq {
	int	(*handler)(void *arg);
	void	*arg;
};

#define VME_NUM_VECTORS		256

/* Mutex to prevent concurrent access to the IRQ table */
static DEFINE_MUTEX(vme_irq_table_lock);
struct vme_irq vme_irq_table[VME_NUM_VECTORS];


/* Interrupt counters */
enum interrupt_idx {
	INT_DMA0 = 0,
	INT_DMA1,
	INT_MB0,
	INT_MB1,
	INT_MB2,
	INT_MB3,
	INT_LM0,
	INT_LM1,
	INT_LM2,
	INT_LM3,
	INT_IRQ1,
	INT_IRQ2,
	INT_IRQ3,
	INT_IRQ4,
	INT_IRQ5,
	INT_IRQ6,
	INT_IRQ7,
	INT_PERR,
	INT_VERR,
	INT_SPURIOUS
};

struct interrupt_stats  {
	unsigned int 	count;
	char		*name;
} int_stats[] = {
	{.name = "DMA0"}, {.name = "DMA1"},
	{.name = "MB0"}, {.name = "MB1"}, {.name = "MB2"}, {.name = "MB3"},
	{.name = "LM0"}, {.name = "LM1"}, {.name = "LM2"}, {.name = "LM3"},
	{.name = "IRQ1"}, {.name = "IRQ2"}, {.name = "IRQ3"}, {.name = "IRQ4"},
	{.name = "IRQ5"}, {.name = "IRQ6"}, {.name = "IRQ7"},
	{.name = "PERR"}, {.name = "VERR"}, {.name = "SPURIOUS"}
};

void account_dma_interrupt(int channel_mask)
{
	if (channel_mask & 1)
		int_stats[INT_DMA0].count++;

	if (channel_mask & 2)
		int_stats[INT_DMA1].count++;
}

static void handle_pci_error(void)
{
	tsi148_handle_pci_error();

	int_stats[INT_PERR].count++;
}

static int
vme_berr_match(struct vme_bus_error *error, struct vme_berr_handler *handler)
{
	struct vme_bus_error *err = &handler->error;

	return error->am == err->am && error->address >= err->address &&
		error->address < err->address + handler->size;
}

static void __vme_dispatch_berr(struct vme_bus_error *error)
{
	struct vme_berr_handler *handler;

	list_for_each_entry(handler, &vme_bridge->verr.h_list, h_list) {
		if (vme_berr_match(error, handler))
			handler->func(error);
	}
}

static void handle_vme_error(void)
{
	struct vme_bus_error_desc desc;
	unsigned long flags;
	spinlock_t *lock;

	lock = &vme_bridge->verr.lock;

	spin_lock_irqsave(lock, flags);
	tsi148_handle_vme_error(&desc.error);
	desc.valid = 1;
	memcpy(&vme_bridge->verr.desc, &desc, sizeof(desc));
	__vme_dispatch_berr(&desc.error);
	spin_unlock_irqrestore(lock, flags);

	int_stats[INT_VERR].count++;
}

static void handle_mbox_interrupt(int mb_mask)
{
	if (mb_mask & 1)
		int_stats[INT_MB0].count++;

	if (mb_mask & 2)
		int_stats[INT_MB1].count++;

	if (mb_mask & 4)
		int_stats[INT_MB2].count++;

	if (mb_mask & 8)
		int_stats[INT_MB3].count++;
}

static void handle_lm_interrupt(int lm_mask)
{
	if (lm_mask & 1)
		int_stats[INT_LM0].count++;

	if (lm_mask & 2)
		int_stats[INT_LM1].count++;

	if (lm_mask & 4)
		int_stats[INT_LM2].count++;

	if (lm_mask & 8)
		int_stats[INT_LM3].count++;
}

/**
 * handle_vme_interrupt() - VME IRQ handler
 * @irq_mask: Mask of the raised IRQs
 *
 *  Get the IRQ vector through an IACK cycle and call the handler for
 * that vector if installed.
 */
static void handle_vme_interrupt(int irq_mask,
				 struct vme_bridge_device *vbridge)
{
	int i, vec, cascade_irq;
	struct irq_desc *desc;

	for (i = 7; i > 0; i--) {

		if (irq_mask & (1 << i)) {
			/* Generate an 8-bit IACK cycle and get the vector */
			vec = tsi148_iack8(vbridge->regs, i);
			if (!vbridge->domain)
				continue;

			cascade_irq = irq_find_mapping(vbridge->domain, vec);
			desc = irq_to_desc(cascade_irq);
			generic_handle_irq(cascade_irq);
		}
	}
}


/**
 * vme_bridge_interrupt() - VME bridge main interrupt handler
 *
 */
irqreturn_t vme_bridge_interrupt(int irq, struct vme_bridge_device *vbridge)
{
	unsigned int raised;
	unsigned int mask;

	/*
	 * We need to read the interrupt status from the VME bus to make
	 * sure the internal FIFO has been flushed of pending writes.
	 */
	while ((raised = tsi148_get_int_status(crg_base)) != 0) {

		/*
		 * Clearing of the interrupts must be done by writing to the
		 * INTS register through the VME bus.
		 */
		tsi148_clear_int(crg_base, raised);

		mask = raised & vme_interrupts_enabled;

		/* Only handle enabled interrupts */
		if (!mask) {
			int_stats[INT_SPURIOUS].count++;
			return IRQ_NONE;
		}

		if (mask & TSI148_LCSR_INT_DMA_M) {
			handle_dma_interrupt((mask & TSI148_LCSR_INT_DMA_M) >>
					     TSI148_LCSR_INT_DMA_SHIFT);
			mask &= ~TSI148_LCSR_INT_DMA_M;
		}

		if (mask & TSI148_LCSR_INT_PERR) {
			handle_pci_error();
			mask &= ~TSI148_LCSR_INT_PERR;
		}

		if (mask & TSI148_LCSR_INT_VERR) {
			handle_vme_error();
			mask &= ~TSI148_LCSR_INT_VERR;
		}

		if (mask & TSI148_LCSR_INT_MB_M) {
			handle_mbox_interrupt((mask & TSI148_LCSR_INT_MB_M) >>
					      TSI148_LCSR_INT_MB_SHIFT);
			mask &= ~TSI148_LCSR_INT_MB_M;
		}

		if (mask & TSI148_LCSR_INT_LM_M) {
			handle_lm_interrupt((mask & TSI148_LCSR_INT_LM_M) >>
					    TSI148_LCSR_INT_LM_SHIFT);
			mask &= ~TSI148_LCSR_INT_LM_M;
		}

		if (mask & TSI148_LCSR_INT_IRQM) {
			handle_vme_interrupt(mask & TSI148_LCSR_INT_IRQM, vbridge);
			mask &= ~TSI148_LCSR_INT_IRQM;
		}

		/* Check that we handled everything */
		if (mask)
			printk(KERN_WARNING PFX
			       "Unhandled interrupt %08x (enabled %08x)\n",
			       mask, vme_interrupts_enabled);
	}

	return IRQ_HANDLED;
}

/**
 * vme_enable_interrupts() - Enable VME bridge interrupts
 * @mask: Interrupts to enable
 *
 */
int vme_enable_interrupts(unsigned int mask)
{
	unsigned int enabled;
	unsigned int new;

	enabled = tsi148_get_int_enabled(vme_bridge->regs);
	new = enabled | mask;

	vme_interrupts_enabled = new;
	return tsi148_set_interrupts(vme_bridge->regs, new);
}

/**
 * vme_disable_interrupts() - Disable VME bridge interrupts
 * @mask: Interrupts to disable
 *
 */
int vme_disable_interrupts(unsigned int mask)
{
	unsigned int enabled;
	unsigned int new;

	enabled = tsi148_get_int_enabled(vme_bridge->regs);
	new = enabled & ~mask;

	vme_interrupts_enabled = new;
	return tsi148_set_interrupts(vme_bridge->regs, new);
}

static irqreturn_t vme_irq_handler(int irq, void *data)
{
	struct vme_irq *virq = data;

	return virq->handler(virq->arg);
}

/**
 * vme_request_irq() - Install handler for a given VME IRQ vector
 * @vec: VME IRQ vector
 * @handler: Interrupt handler
 * @arg: Interrupt handler argument
 * @name: Interrupt name (only used for stats in Procfs)
 *
 */
int vme_request_irq(unsigned int vec, int (*handler)(void *),
		    void *arg, const char *name)
{
	struct vme_irq *virq;
	int rc = 0;

	/* Check the vector is within the bound */
	if (vec >= VME_NUM_VECTORS)
		return -EINVAL;

	rc = mutex_lock_interruptible(&vme_irq_table_lock);
	if (rc)
		return rc;

	virq = &vme_irq_table[vec];

	if (virq->handler) {
		mutex_unlock(&vme_irq_table_lock);
		return -EBUSY;
	}
	virq->handler = handler;
	virq->arg = arg;

	mutex_unlock(&vme_irq_table_lock);

	return request_irq(irq_find_mapping(vme_bridge->domain, vec),
			   vme_irq_handler, 0, name, virq);
}
EXPORT_SYMBOL_GPL(vme_request_irq);

/**
 * vme_free_irq() - Uninstall handler for a given VME IRQ vector
 * @vec: VME IRQ vector
 *
 */
int vme_free_irq(unsigned int vec)
{
	struct vme_irq *virq;
	int rc = 0;

	/* Check the vector is within the bound */
	if (vec >= VME_NUM_VECTORS)
		return -EINVAL;

	rc = mutex_lock_interruptible(&vme_irq_table_lock);
	if (rc)
		return rc;

	virq = &vme_irq_table[vec];

	if (!virq->handler) {
		mutex_unlock(&vme_irq_table_lock);
		return -EINVAL;
	}

	free_irq(irq_find_mapping(vme_bridge->domain, vec),
		 virq);

	virq->handler = NULL;
	virq->arg = NULL;
	mutex_unlock(&vme_irq_table_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(vme_free_irq);

/**
 * vme_generate_interrupt() - Generate an interrupt on the VME bus
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
int vme_generate_interrupt(int level, int vector, signed long msecs)
{
	return tsi148_generate_interrupt(level, vector, msecs);
}
EXPORT_SYMBOL_GPL(vme_generate_interrupt);
