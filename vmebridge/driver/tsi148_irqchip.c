/*
 * tsi148.h - Low level support for the Tundra TSI148 PCI-VME Bridge Chip
 *
 * Copyright (c) 2016 CERN
 * Author: Federico Vaga <federico.vaga@cern.ch>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/version.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#include <linux/irqchip/chained_irq.h>
#else
static inline void chained_irq_enter(struct irq_chip *chip,
                                     struct irq_desc *desc)
{
        /* FastEOI controllers require no action on entry. */
        if (chip->irq_eoi)
                return;

        if (chip->irq_mask_ack) {
                chip->irq_mask_ack(&desc->irq_data);
        } else {
                chip->irq_mask(&desc->irq_data);
                if (chip->irq_ack)
                        chip->irq_ack(&desc->irq_data);
        }
}

static inline void chained_irq_exit(struct irq_chip *chip,
                                    struct irq_desc *desc)
{
        if (chip->irq_eoi)
                chip->irq_eoi(&desc->irq_data);
        else
                chip->irq_unmask(&desc->irq_data);
}
#endif

#include "vme_bridge.h"


/**
 * It unmask (a.k.a. enable) an interrupt line
 */
void tsi148_irq_unmask(struct irq_data *data)
{
	//printk("%s:%d\n", __func__, __LINE__);
}

/**
 * It mask (a.k.a. disable) an interrupt line
 */
void tsi148_irq_mask(struct irq_data *data)
{
	//printk("%s:%d\n", __func__, __LINE__);
}

/**
 * It acknowledge an interrupt
 */
void tsi148_irq_ack(struct irq_data *data)
{
}

/*
 * IRQ Chip for Tundra TSI148 devices.
 */
static struct irq_chip tsi148_chip = {
	.name			= "TSI148",
	.irq_unmask		= tsi148_irq_unmask,
	.irq_mask		= tsi148_irq_mask,
	.irq_ack		= tsi148_irq_ack,
};


static int tsi148_irq_domain_map(struct irq_domain *h,
				 unsigned int virtirq,
				 irq_hw_number_t hwirq)
{
	struct vme_bridge *vbridge = h->host_data;
	struct irq_desc *desc = irq_to_desc(virtirq);

	irq_set_chip(virtirq, &tsi148_chip);

	/*
	 * It MUST be no-thread because the VIC EOI must occur AFTER
	 * the device handler ack its signal. Any whay the interrupt from
	 * the carrier is already threaded
	 */
	desc->status_use_accessors |= IRQ_NOTHREAD;

	__irq_set_handler(virtirq, handle_edge_irq, 0, "edge");
	irq_set_chip_data(virtirq, vbridge);
	irq_set_handler_data(virtirq, vbridge);

	return 0;
}

static struct irq_domain_ops tsi148_irq_domain_ops = {
	.map = tsi148_irq_domain_map,
};


/**
 * Handle cascade IRQ coming from the TSI148 and re-route it properly
 * When the TSI148 receives an interrupt it will call this function
 * which then will call the proper handler
 */
static void tsi148_handle_cascade_irq(unsigned int irq, struct irq_desc *desc)
{
	struct vme_bridge_device *vbridge = irq_get_handler_data(irq);
	struct irq_chip *chip = irq_get_chip(irq);

	chained_irq_enter(chip, desc);
	vme_bridge_interrupt(irq, vbridge);
	chained_irq_exit(chip, desc);
}


/**
 * Configure the TSI148 to be chained to the standard PCI irq controller
 */
int tsi148_irq_domain_create(struct vme_bridge_device *vbridge)
{
	unsigned int irq;
	int i, err;

	vbridge->domain = irq_domain_add_linear(NULL, VME_NUM_VECTORS,
						&tsi148_irq_domain_ops, vbridge);
	if (!vbridge->domain)
		return -ENOMEM;

	/* Maps HW interrupt number with Linux IRQ number*/
	for (i = 0; i < VME_NUM_VECTORS; ++i) {
		irq = irq_create_mapping(vbridge->domain, i);
		if (!irq)
			goto out;
		if (i == 0)
			vbridge->base_irq = irq;
	}

	err = irq_set_handler_data(vbridge->irq, vbridge);
	if (err)
		BUG();
	irq_set_chained_handler(vbridge->irq, tsi148_handle_cascade_irq);

	return 0;

out:
	irq_domain_remove(vbridge->domain);
	vbridge->domain = NULL;
	return -EPERM;
}

void tsi148_irq_domain_destroy(struct vme_bridge_device *vbridge)
{
	irq_domain_remove(vbridge->domain);
	vbridge->domain = NULL;
}
