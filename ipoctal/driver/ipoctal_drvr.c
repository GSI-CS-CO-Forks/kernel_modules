/**
 * ipoctal_drvr.c
 *
 * driver for the IPOCTAL boards
 * Copyright (c) 2009 Nicolas Serafini, EIC2 SA
 * Copyright (c) 2010,2011 Samuel Iglesias Gonsalvez <siglesia@cern.ch>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/tty_flip.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "ipoctal_drvr.h"
#include "carrier.h"
#include "scc2698.h"

static int lun[IPOCTAL_MAX_BOARDS];
static unsigned int num_lun;
module_param_array(lun, int, &num_lun, S_IRUGO);
MODULE_PARM_DESC(lun, "Logical Unit Number of the ipoctal board");

static int carrier_number[IPOCTAL_MAX_BOARDS];
static unsigned int num_carrier_number;
module_param_array(carrier_number, int, &num_carrier_number, S_IRUGO);
MODULE_PARM_DESC(carrier_number, "Logical Unit Number of the carrier");

static int slot[IPOCTAL_MAX_BOARDS];
static unsigned int num_slot;
module_param_array(slot, int, &num_slot, S_IRUGO);
MODULE_PARM_DESC(slot, "Slot of the carrier in which the ipoctal board is placed.");

static int irq[IPOCTAL_MAX_BOARDS];
static unsigned int num_irq;
module_param_array(irq, int, &num_irq, S_IRUGO);
MODULE_PARM_DESC(irq, "IRQ vector of the ipoctal board.");

static char *carrier[IPOCTAL_MAX_BOARDS];
static unsigned int num_carrier;
module_param_array(carrier, charp, &num_carrier, S_IRUGO);
MODULE_PARM_DESC(carrier, "Name of the carrier in which the IPOCTAL-XXX is plugged in.");

static int major[IPOCTAL_MAX_BOARDS];
static unsigned int num_major;
module_param_array(major, int, &num_major, S_IRUGO);
MODULE_PARM_DESC(major, "Overwrite the default major (260 + index) for the TTY_IPOCTAL");



typedef enum {
	CHAN_CLOSE  = 0,
	CHAN_OPEN   = 1,
	CHAN_READ   = 2,
	CHAN_WRITE  = 3,
} chan_status;

#define IPOCTAL_DRIVER_DESCRIPTION "IP Octal 232, 422 and 485 driver"
#define IPOCTAL_DRIVER_VERSION "2.0" 
#define IPOCTAL_DRIVER_AUTHOR "Nicolas Serafini, EIC2 SA."
#define IPOCTAL_DRIVER_LICENSE "GPL"

#define MODULE_NAME "ipoctal"
#define PFX MODULE_NAME ": "
#define IP_OCTAL_MANUFACTURER_ID    0xF0
#define IP_OCTAL_232_ID             0x22
#define IP_OCTAL_422_ID             0x2A
#define IP_OCTAL_485_ID             0x48

#define IP_OCTAL_ID_SPACE_VECTOR    0x41
#define IP_OCTAL_NB_BLOCKS          4
#define DELAY_MICROSECONDS          1
#define MAX_CHAR		    256

struct ipoctal {
	struct slot_id			*slot_id;
	unsigned char			board_id;
	unsigned int			index;
	char				name[NAME_MAX];
	struct scc2698_channel		*chan_regs;
	struct scc2698_block		*block_regs;
	struct ipoctal_stats		chan_stats[NR_CHANNELS];
	chan_status			chan_status[NR_CHANNELS];
	struct ipoctal_readwrite	read_write[NR_CHANNELS];
	char				*buffer[NR_CHANNELS];
	unsigned int			nb_bytes[NR_CHANNELS];
	unsigned int			count_wr[NR_CHANNELS];
	struct ipoctal_config		chan_config[NR_CHANNELS];
	wait_queue_head_t		queue[NR_CHANNELS];
	uart_error			error_flag[NR_CHANNELS];
	unsigned int			minor[NR_CHANNELS];
	spinlock_t			lock[NR_CHANNELS];
	struct mutex			lock_write[NR_CHANNELS];
	unsigned int			pointer_read[NR_CHANNELS];
	unsigned int			pointer_write[NR_CHANNELS];		
	unsigned int			open[NR_CHANNELS];
	unsigned char			read;
	unsigned char			write;
	struct tty_struct 		*tty[NR_CHANNELS];
	struct ktermios			oldtermios[NR_CHANNELS];
	struct tty_driver		*tty_drv;
};

static int __init ipoctal_init(void);
static void __exit ipoctal_exit(void);
int ipoctal_open(struct tty_struct *tty, struct file *filp);
void ipoctal_close(struct tty_struct *tty, struct file *filp);
int ipoctal_ioctl_compat(struct tty_struct *tty, struct file *file, unsigned int cmd, unsigned long arg);
int ipoctal_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);
static int ipoctal_write_tty(struct tty_struct *tty, const unsigned char *buf, int count);
static int ipoctal_inst_slot(struct ipoctal *ipoctal, unsigned int carrier_number, unsigned int slot_position, unsigned int vector, char *carrier);
static void ipoctal_uninst_slot(struct ipoctal *ipoctal);
static int ipoctal_install_all(void);
static void ipoctal_reset_stats(struct ipoctal_stats *stats);
static int ipoctal_irq_handler(void *arg);
static int ipoctal_write(struct ipoctal *ipoctal, unsigned int channel, const unsigned char *buf, int count);
static void ipoctal_free_channel(struct tty_struct *tty);
inline void ipoctal_write_io_reg(struct ipoctal *ipoctal, unsigned char *dest, unsigned char value);
inline void ipoctal_write_cr_cmd(struct ipoctal *ipoctal, unsigned char *dest, unsigned char value);
inline unsigned char ipoctal_read_io_reg(struct ipoctal *ipoctal, unsigned char *src);
void ipoctal_set_termios(struct tty_struct *tty, struct ktermios *old_termios);
int ipoctal_chars_in_buffer(struct tty_struct *tty);
int ipoctal_write_room(struct tty_struct *tty);

static struct ipoctal *ipoctal_installed = NULL;

MODULE_DESCRIPTION(IPOCTAL_DRIVER_DESCRIPTION);
MODULE_AUTHOR(IPOCTAL_DRIVER_AUTHOR);
MODULE_LICENSE(IPOCTAL_DRIVER_LICENSE);
MODULE_VERSION(IPOCTAL_DRIVER_VERSION);
MODULE_VERSION(GIT_VERSION);

module_init(ipoctal_init);
module_exit(ipoctal_exit);


struct tty_operations ipoctalFops =
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38)
	.ioctl = 		ipoctal_ioctl_compat,
#else
	.ioctl =		ipoctal_ioctl,
#endif
	.open = 		ipoctal_open,
	.close = 		ipoctal_close,
	.write =		ipoctal_write_tty,
	.set_termios =		ipoctal_set_termios,
	.write_room =		ipoctal_write_room,
	.chars_in_buffer =	ipoctal_chars_in_buffer,
};

static int __init ipoctal_init(void)
{

	printk(KERN_ERR PFX "IP octal driver loading ( %s )...\n", MODULE_NAME);
	if (num_lun != num_irq || num_lun != num_carrier_number ||
			num_lun != num_slot || num_lun >= IPOCTAL_MAX_BOARDS) {
		printk(KERN_ERR PFX "the number of parameters doesn't match or are invalid.\n");
		return -1;
	}
	ipoctal_install_all();
	printk(KERN_ERR PFX "IP octal driver loaded ( %s ).\n", MODULE_NAME);
	return 0;
}

static void __exit ipoctal_exit(void)
{
	int i, j;

	printk(KERN_INFO PFX "IP octal driver unloading... ( %s )\n", MODULE_NAME);
	
	if(ipoctal_installed == NULL)
		goto print_exit;	

	for (i=0;i<num_lun;i++) {
		if (ipoctal_installed[i].tty_drv == NULL)
			continue;

		for (j=0; j < NR_CHANNELS; j++) {
			tty_unregister_device(ipoctal_installed[i].tty_drv, j);
			kfree(ipoctal_installed[i].buffer[j]);
		}
		ipoctal_uninst_slot(&ipoctal_installed[i]);
		tty_unregister_driver(ipoctal_installed[i].tty_drv);
	}

	if (ipoctal_installed != NULL) {
		kfree(ipoctal_installed);
	}

print_exit:
	printk(KERN_INFO PFX "IP octal driver unloaded ( %s )\n", MODULE_NAME);
}

int ipoctal_find_device (struct tty_struct *tty)
{
	int i;
	for(i = 0; i < num_lun; i++){
		if(tty->driver->major == ipoctal_installed[i].tty_drv->major)
			return i;
	}

	return -1;
}

int ipoctal_open(struct tty_struct *tty, struct file *file)
{
	int channel = tty->index;
	int major = tty->driver->major;
	int res = 0;
	int index;

	index = ipoctal_find_device(tty);
	
	if (index < 0) {
		printk(KERN_ERR PFX "Device not found. Major %d\n", major);
		return -ENODEV;
	}

	ipoctal_installed[index].open[channel]++;
	if (ipoctal_installed[index].open[channel] > 1){
		return -EBUSY;
	}

	ipoctal_installed[index].tty[channel] = tty;
	memcpy(&ipoctal_installed[index].oldtermios[channel], tty->termios, sizeof(struct ktermios));
	ipoctal_write_io_reg(
		&ipoctal_installed[index], 
		&ipoctal_installed[index].chan_regs[channel].u.w.cr, 
		CR_ENABLE_RX);

	tty->driver_data = &ipoctal_installed[index].index;

	
	return res;
}

void ipoctal_close(struct tty_struct *tty, struct file *filp)
{
	int channel = tty->index;
	int index = *(int *)tty->driver_data;

	ipoctal_installed[index].open[channel]--;

	if(!ipoctal_installed[index].open[channel]) {
		ipoctal_free_channel(tty);
		ipoctal_installed[index].tty[channel] = NULL;
	}
}

inline int ipoctal_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
{
	return ipoctal_ioctl_compat(tty, NULL, cmd, arg);
}

int ipoctal_ioctl_compat(struct tty_struct *tty, struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *user_arg = (void __user *)arg;
	int index = *(int *)tty->driver_data;
	struct ipoctal *ipoctal;
	int channel = tty->index;
	int res = -ENOIOCTLCMD;

	ipoctal = &ipoctal_installed[index];

	switch(cmd){
	case TIOCGICOUNT :
	{
		struct serial_icounter_struct icount;

		if (channel < 0) {
			res = channel;
			goto out_ioctl;
		}

		icount.cts = 0;
		icount.dsr = 0;
		icount.rng = 0;
		icount.dcd = 0;
		icount.rx = ipoctal->chan_stats[channel].rx;
		icount.tx = ipoctal->chan_stats[channel].tx;
		icount.frame = ipoctal->chan_stats[channel].framing_err;
		icount.parity = ipoctal->chan_stats[channel].parity_err;
		icount.brk = ipoctal->chan_stats[channel].rcv_break;

		if (copy_to_user(user_arg, &icount, sizeof(icount))){
			printk(KERN_ERR PFX "Slot [%d:%d] Channel %c :"
				" Error during data copy to user space !\n",
				ipoctal->slot_id->carrier_number, 
				ipoctal->slot_id->slot_position, 'a'+channel);
			res = -EFAULT;
			goto out_ioctl;
		}
		break;
	}
	default:
		res = -ENOIOCTLCMD;
		goto out_ioctl;
		break;
	}
out_ioctl :
	return res;
}

static int ipoctal_inst_slot(struct ipoctal *ipoctal, unsigned int carrier_number, unsigned int slot_position, unsigned int vector, char *carrier)
{
	char name[NAME_MAX];
	unsigned char manufacturerID;
	int res = 0;
	int i;
	int model = 0;

	ipoctal->slot_id = NULL;
	ipoctal->block_regs = NULL;
	ipoctal->chan_regs = NULL;

	for(i=0; i<NR_CHANNELS ; i++){
		ipoctal_reset_stats(&ipoctal->chan_stats[i]);
		ipoctal->chan_status[i] = CHAN_CLOSE;
		ipoctal->read_write[i].buffer = 0;
		ipoctal->read_write[i].error_flag = UART_NOERROR;
		ipoctal->read_write[i].nb_bytes = 0;
		ipoctal->read_write[i].timeout = 0;
		ipoctal->buffer[i] = kzalloc(MAX_CHAR*sizeof(unsigned char), GFP_KERNEL);
		ipoctal->nb_bytes[i] = 0;
		init_waitqueue_head(&ipoctal->queue[i]);
		ipoctal->error_flag[i] = UART_NOERROR;
		ipoctal->minor[i] = 0;
	}

	/* Register the device */
	sprintf(name, "%s_%d_%d", MODULE_NAME, carrier_number, slot_position);
	ipoctal->slot_id = ip_slot_register(name, carrier_number, slot_position);

	if (ipoctal->slot_id == NULL){
		printk(KERN_ERR PFX "Unable to install slot [%d:%d]!\n", carrier_number, slot_position);
		res = -ENXIO;
		goto out_err;
	}

	/* Map the corresponding spaces */
	res = ip_slot_map_space(ipoctal->slot_id, 0, SLOT_ID_SPACE);
	if (res){
		printk(KERN_ERR PFX "Unable to map slot [%d:%d] ID space!\n", carrier_number, slot_position);
		goto out_unregisterSlot;
	}

	res = ip_slot_map_space(ipoctal->slot_id, 0, SLOT_IO_SPACE);
	if (res){
		printk(KERN_ERR PFX "Unable to map slot [%d:%d] IO space!\n", carrier_number, slot_position);
		goto out_unregisterSlot;
	}

	res = ip_slot_map_space(ipoctal->slot_id, 0x8000, SLOT_MEM_SPACE);
	if (res){
		printk(KERN_ERR PFX "Unable to map slot [%d:%d] MEM space!\n", carrier_number, slot_position);
		goto out_unregisterSlot;
	}
	ipoctal->chan_regs = (struct scc2698_channel *) ipoctal->slot_id->io_space.address;
	ipoctal->block_regs = (struct scc2698_block *) ipoctal->slot_id->io_space.address;

	/* Check if the HW is managed by this driver */
	ip_slot_read_uchar(&manufacturerID, ipoctal->slot_id, SLOT_ID_SPACE, 
					SLOT_IDPROM_OFFSET_MANUFACTURER_ID);

	if (manufacturerID != IP_OCTAL_MANUFACTURER_ID) {
		printk(KERN_ERR PFX "Manufacturer ID of slot [%d:%d] must be 0x%X and not 0x%X !\n",
				carrier_number, slot_position,
				IP_OCTAL_MANUFACTURER_ID, manufacturerID);
		res = -ENODEV;
		goto out_unregisterSlot;
	}

	ip_slot_read_uchar(&ipoctal->board_id, ipoctal->slot_id, SLOT_ID_SPACE, 
							SLOT_IDPROM_OFFSET_MODEL);
	switch(ipoctal->board_id) {
	case IP_OCTAL_232_ID :
		printk(KERN_INFO PFX "Slot [%d:%d] is a RS-232 %d channels.\n",
				carrier_number, slot_position, NR_CHANNELS);
		model = 232;
		break;
	case IP_OCTAL_422_ID :
		printk(KERN_INFO PFX "Slot [%d:%d] is a RS-422 %d channels.\n",
				carrier_number, slot_position, NR_CHANNELS);
		model = 422;
		break;
	case IP_OCTAL_485_ID :
		printk(KERN_INFO PFX "Slot [%d:%d] is a RS-485 %d channels.\n",
				carrier_number, slot_position, NR_CHANNELS);
		model = 485;
	break;
	default :
		printk(KERN_ERR PFX "Model ID (0x%X) of slot [%d:%d] is unknown !\n",
				ipoctal->board_id, carrier_number, slot_position);
		res = -ENODEV;
		goto out_unregisterSlot;
		break;
	}

	for(i=0; i<IP_OCTAL_NB_BLOCKS ; i++){
		ipoctal_write_io_reg(ipoctal, &ipoctal->block_regs[i].u.w.acr, ACR_BRG_SET2);
		ipoctal_write_io_reg(
				ipoctal, 
				&ipoctal->block_regs[i].u.w.opcr, 
				OPCR_MPP_OUTPUT | OPCR_MPOa_RTSN | OPCR_MPOb_RTSN);
		ipoctal_write_io_reg(
				ipoctal, 
				&ipoctal->block_regs[i].u.w.imr,
				IMR_TxRDY_A | IMR_RxRDY_FFULL_A | IMR_DELTA_BREAK_A |
				IMR_TxRDY_B | IMR_RxRDY_FFULL_B | IMR_DELTA_BREAK_B);
	}

	for(i=0; i<NR_CHANNELS ; i++){
		ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[i].u.w.cr, 
						CR_DISABLE_RX | CR_DISABLE_TX);
	}

	/* Install IRQ */

	/* IP-OCTAL has different addresses to copy its IRQ vector. Depending of the carrier
         * these addresses are accesible or not. More data in the datasheet.
	 */

	if (strcmp(carrier, "TVME200") == 0)
		ip_slot_write_vector(vector, ipoctal->slot_id, SLOT_ID_SPACE, IP_OCTAL_ID_SPACE_VECTOR);
	else
		ip_slot_write_vector(vector, ipoctal->slot_id, SLOT_MEM_SPACE, 0);

	res = ip_slot_request_irq(ipoctal->slot_id, vector, ipoctal_irq_handler, (void *) ipoctal, name);
	if (res){
		printk(KERN_ERR PFX "Unable to install IRQ of slot [%d:%d]!\n", carrier_number, slot_position);
		goto out_unregisterSlot;
	}

	return 0;

out_unregisterSlot :
	ip_slot_unregister(ipoctal->slot_id);
out_err :
	return res;
}

static void ipoctal_uninst_slot(struct ipoctal *ipoctal)
{
	if (ipoctal->slot_id != NULL) {
		ip_slot_unregister(ipoctal->slot_id);
	}
}

static int ipoctal_install_all(void)
{
	int i, j, t;
	int res = 0;
	struct tty_driver *tty;
	char name[20] = "";

	ipoctal_installed = (struct ipoctal*) kzalloc(num_lun * sizeof(struct ipoctal), GFP_KERNEL);
	if (ipoctal_installed == NULL) {
		printk(KERN_ERR PFX "Unable to allocate memory for ipoctal's !\n");
		res = -ENOMEM;
		goto out_err;
	}

	for (i=0; i<num_lun;i++) {
		tty = alloc_tty_driver(NR_CHANNELS);

		if(!tty)
			return -ENOMEM;

		tty->owner = THIS_MODULE;
		tty->driver_name = "ipoctal";
		sprintf(name, "ipoctal.%d.", lun[i]);
		tty->name = name;
		tty->major = 0;

		tty->minor_start = 0;
		tty->type = TTY_DRIVER_TYPE_SERIAL;
		tty->subtype = SERIAL_TYPE_NORMAL;
		tty->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
		tty->init_termios = tty_std_termios;
		tty->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
		tty->init_termios.c_ispeed = 9600;
		tty->init_termios.c_ospeed = 9600;
		tty->init_termios.c_iflag = tty_std_termios.c_iflag | IGNBRK;

		tty_set_operations(tty, &ipoctalFops);
		res = tty_register_driver(tty);
		if(res) {
			printk(KERN_ERR PFX "Can't register tty driver.\n");
			put_tty_driver(tty);
			goto out_uninst;
		}

		for(j = 0; j < NR_CHANNELS;j++) {
			tty_register_device(tty, j, NULL);
			ipoctal_installed[i].tty[j] = NULL;
			spin_lock_init(&ipoctal_installed[i].lock[j]);
			mutex_init(&ipoctal_installed[i].lock_write[j]);
			ipoctal_installed[i].pointer_read[j] = 0;
			ipoctal_installed[i].pointer_write[j] = 0;
			ipoctal_installed[i].nb_bytes[j] = 0;
		}
		ipoctal_installed[i].tty_drv = tty;
		ipoctal_installed[i].index = i;

		res = ipoctal_inst_slot(&ipoctal_installed[i],
				carrier_number[i],
				slot[i],
				irq[i],
				carrier[i]);
		if (res) {
			printk(KERN_ERR PFX "Error during IP octal install !\n");
			goto out_uninst;
		}	
	}

	return 0;

out_uninst :
	for (j=0; j < i;j++){
		for (t = 0; t < NR_CHANNELS; t++)
			tty_unregister_device(ipoctal_installed[j].tty_drv, t);
		tty_unregister_driver(ipoctal_installed[j].tty_drv);
		ipoctal_uninst_slot(&ipoctal_installed[j]);
	}
	kfree(ipoctal_installed);
	ipoctal_installed = NULL;
	printk(KERN_ERR PFX "Unregistered all IP octal devices\n");
out_err :
	return res;
}

static int ipoctal_write_tty(struct tty_struct *tty, const unsigned char *buf, int count)
{
	int index = *(int *)tty->driver_data;
	unsigned int channel = tty->index;
	struct ipoctal *ipoctal = &ipoctal_installed[index];
	int ret = 0;

	/* Serialize the access to the HW per each channel */
	if (mutex_lock_interruptible(&ipoctal->lock_write[channel]))
		return -ERESTARTSYS;

	ret = ipoctal_write(ipoctal, channel, buf, count);
	mutex_unlock(&ipoctal->lock_write[channel]);
	return ret;
}

static inline int ipoctal_copy_write_buffer(struct ipoctal *ipoctal, unsigned int channel, const unsigned char *buf, int count) 
{
	unsigned long flags;
	int i;
	unsigned int *pointer_read = &ipoctal->pointer_read[channel];

	for (i=0; i<count; i++) {
		if ( i <= (MAX_CHAR - ipoctal->nb_bytes[channel])) {
			spin_lock_irqsave(&ipoctal->lock[channel], flags);
			ipoctal->buffer[channel][*pointer_read] = buf[i];
			*pointer_read = (*pointer_read + 1) % MAX_CHAR;
			ipoctal->nb_bytes[channel]++;
			spin_unlock_irqrestore(&ipoctal->lock[channel], flags);
		}
		else {
			break;
		}
	}
	return i;
}

static int ipoctal_write(struct ipoctal *ipoctal, unsigned int channel, const unsigned char *buf, int count)
{
	ipoctal->nb_bytes[channel] = 0;
	ipoctal->count_wr[channel] = 0;

	/* Copy the data to the write buffer */
	ipoctal_copy_write_buffer(ipoctal, channel, buf, count);

	ipoctal->error_flag[channel] = UART_NOERROR;
	ipoctal->read_write[channel].error_flag = UART_NOERROR;

	/* Using half-duplex on transmission */
	ipoctal->chan_status[channel] = CHAN_WRITE;
	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_DISABLE_RX);

	if (ipoctal->board_id == IP_OCTAL_485_ID) {
		ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
								CR_CMD_ASSERT_RTSN);
	}

	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_ENABLE_TX);
	wait_event_interruptible(ipoctal->queue[channel], ipoctal->write);

	/* End Write operation */
	ipoctal->write = 0;
	ipoctal->chan_status[channel] = CHAN_READ;

	if(ipoctal->board_id != IP_OCTAL_485_ID) {
		ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_DISABLE_TX);
		ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_ENABLE_RX);
	}

	return ipoctal->count_wr[channel];
}

int ipoctal_write_room(struct tty_struct *tty)
{
	int index = *(int *)tty->driver_data;
	int channel = tty->index;
	struct ipoctal *ipoctal = &ipoctal_installed[index];

	return (MAX_CHAR - ipoctal->nb_bytes[channel]);
}

int ipoctal_chars_in_buffer(struct tty_struct *tty)
{
	int index = *(int *)tty->driver_data;
	int channel = tty->index;
	struct ipoctal *ipoctal = &ipoctal_installed[index];

	return ipoctal->nb_bytes[channel];
}

static int ipoctal_irq_handler(void *arg)
{
	unsigned int channel;
	unsigned int block;
	unsigned char isr;
	unsigned char sr;
	unsigned char isrTxRdy, isrRxRdy;
	unsigned char value;
	struct ipoctal *ipoctal = (struct ipoctal *) arg;

	for (channel=0; channel<NR_CHANNELS; channel++) {

		/* The HW is organized in pair of channels. See which register we need to read from */
		block = channel / 2;
		isr = ipoctal_read_io_reg(ipoctal, &ipoctal->block_regs[block].u.r.isr);
		sr = ipoctal_read_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.r.sr);

		if((channel % 2) == 1) {
			isrTxRdy = isr & ISR_TxRDY_B;
			isrRxRdy = isr & ISR_RxRDY_FFULL_B;
		} else {
			isrTxRdy = isr & ISR_TxRDY_A;
			isrRxRdy = isr & ISR_RxRDY_FFULL_A;
		}

		/* In case of RS-485, change from TX to RX. Half-duplex. */
		if ((ipoctal->board_id == IP_OCTAL_485_ID) && (sr & SR_TX_EMPTY) && (ipoctal->nb_bytes[channel] == 0)) {
			ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_DISABLE_TX);
			ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_CMD_NEGATE_RTSN);
			ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, CR_ENABLE_RX);
			ipoctal->write = 1;
			wake_up_interruptible(&ipoctal->queue[channel]);
		}

		/* RX data */
		if (isrRxRdy && (sr & SR_RX_READY) && (ipoctal->chan_status[channel] == CHAN_READ)) {
			value = ipoctal_read_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.r.rhr);
			tty_insert_flip_char(ipoctal->tty[channel], value, TTY_NORMAL);
			tty_flip_buffer_push(ipoctal->tty[channel]);
		}

		/* TX of each character */
		if (isrTxRdy && (sr & SR_TX_READY) && (ipoctal->chan_status[channel] == CHAN_WRITE)) {
			unsigned int *pointer_write = &ipoctal->pointer_write[channel];

			if(ipoctal->nb_bytes[channel] <= 0) {
				ipoctal->nb_bytes[channel] = 0;
				continue;
			}
			spin_lock(&ipoctal->lock[channel]);
			value = ipoctal->buffer[channel][*pointer_write];
			ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.thr, value);
			ipoctal->chan_stats[channel].tx++;
			ipoctal->count_wr[channel]++;
			(*pointer_write)++;
			*pointer_write = *pointer_write % MAX_CHAR;
			ipoctal->nb_bytes[channel]--;
			spin_unlock(&ipoctal->lock[channel]);

			if ((ipoctal->nb_bytes[channel] == 0) &&
					(waitqueue_active(&ipoctal->queue[channel]))){

				if (ipoctal->board_id != IP_OCTAL_485_ID) {
					ipoctal->write = 1;
					wake_up_interruptible(&ipoctal->queue[channel]);
				} else {
					ipoctal->chan_status[channel] = CHAN_OPEN;
				}
			}
		}

		/* Error: count statistics */
		if (sr & SR_ERROR) {
			ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
									CR_CMD_RESET_ERR_STATUS);
			if (sr & SR_OVERRUN_ERROR){
				ipoctal->error_flag[channel] |= UART_OVERRUN;
				ipoctal->chan_stats[channel].overrun_err++;
			}
			if (sr & SR_PARITY_ERROR){
				ipoctal->error_flag[channel] |= UART_PARITY;
				ipoctal->chan_stats[channel].parity_err++;
			}
			if (sr & SR_FRAMING_ERROR){
				ipoctal->error_flag[channel] |= UART_FRAMING;
				ipoctal->chan_stats[channel].framing_err++;
			}
			if (sr & SR_RECEIVED_BREAK){
				ipoctal->error_flag[channel] |= UART_BREAK;
				ipoctal->chan_stats[channel].rcv_break++;
			}
			if (waitqueue_active(&ipoctal->queue[channel])){
				ipoctal->chan_status[channel] = CHAN_OPEN;
				wake_up_interruptible(&ipoctal->queue[channel]);
			}
		}
	}
	return IRQ_HANDLED;
}

static void ipoctal_reset_stats(struct ipoctal_stats *stats)
{
	stats->tx = 0;
	stats->rx = 0;
	stats->rcv_break = 0;
	stats->framing_err = 0;
	stats->overrun_err = 0;
	stats->parity_err = 0;
}

static void ipoctal_free_channel(struct tty_struct *tty)
{
	int index = *(int *)tty->driver_data;
	int channel = tty->index;
	struct ipoctal *ipoctal = &ipoctal_installed[index];

	if (ipoctal != NULL) {
		ipoctal_reset_stats(&ipoctal->chan_stats[channel]);
		ipoctal->chan_status[channel] = CHAN_CLOSE;
		ipoctal->minor[channel] = 0;

		ipoctal->pointer_read[channel] = 0;
		ipoctal->pointer_write[channel] = 0;
		ipoctal->nb_bytes[channel] = 0;
	}
	else
		printk(KERN_ERR PFX "ipoctal_free_channel : ipoctal == NULL, is it"
				"installed? \n");
}

void ipoctal_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
{
	unsigned int cflag;
	unsigned char mr1 = 0;
	unsigned char mr2 = 0;
	unsigned char csr = 0;
 	unsigned char imr = 0;
 	unsigned char imr_brk = 0;
	unsigned int channel = tty->index;
	int index = *(int *)tty->driver_data;
	struct ipoctal *ipoctal;
 	int block = channel / 2;

	ipoctal = &ipoctal_installed[index];

	cflag = tty->termios->c_cflag;

	if (old_termios) {
		if((cflag == old_termios->c_cflag) &&
				(RELEVANT_IFLAG(tty->termios->c_iflag) ==
				 RELEVANT_IFLAG(old_termios->c_iflag)))
			return;
	}

	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
						CR_DISABLE_RX | CR_DISABLE_TX);
	ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
								CR_CMD_RESET_RX);
	ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
								CR_CMD_RESET_TX);
	ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
								CR_CMD_RESET_ERR_STATUS);
	ipoctal_write_cr_cmd(ipoctal, &ipoctal->chan_regs[channel].u.w.cr, 
								CR_CMD_RESET_MR);

	/* Set Bits per chars */
	switch(cflag & CSIZE) {
		case CS6 : mr1 |= MR1_CHRL_6_BITS; break;
		case CS7 : mr1 |= MR1_CHRL_7_BITS; break;
		default:
		case CS8 : mr1 |= MR1_CHRL_8_BITS; break;
			   break;
	}

	/* Set Parity */
	if (cflag & PARENB)
		if(cflag & PARODD)
			mr1 |= MR1_PARITY_ON | MR1_PARITY_ODD;
		else
			mr1 |= MR1_PARITY_ON | MR1_PARITY_EVEN;
	else
		mr1 |= MR1_PARITY_OFF;

	/* Set stop bits */
	if (cflag & CSTOPB)
		mr2 |= MR2_STOP_BITS_LENGTH_2;
	else
		mr2 |= MR2_STOP_BITS_LENGTH_1;

	/* Set the flow control */
	switch(ipoctal->board_id) {
		case IP_OCTAL_232_ID :
			if (cflag & CRTSCTS) {
				mr1 |= MR1_RxRTS_CONTROL_ON;
				mr2 |= MR2_TxRTS_CONTROL_OFF | MR2_CTS_ENABLE_TX_ON;
				ipoctal->chan_config[channel].flow_control = 1;
			}
			else {
				mr1 |= MR1_RxRTS_CONTROL_OFF;
				mr2 |= MR2_TxRTS_CONTROL_OFF | MR2_CTS_ENABLE_TX_OFF;
				ipoctal->chan_config[channel].flow_control = 0;
			}
			break;
		case IP_OCTAL_422_ID :
			mr1 |= MR1_RxRTS_CONTROL_OFF;
			mr2 |= MR2_TxRTS_CONTROL_OFF | MR2_CTS_ENABLE_TX_OFF;
			ipoctal->chan_config[channel].flow_control = 0;
			break;
		case IP_OCTAL_485_ID :
			mr1 |= MR1_RxRTS_CONTROL_OFF;
			mr2 |= MR2_TxRTS_CONTROL_ON | MR2_CTS_ENABLE_TX_OFF;
			ipoctal->chan_config[channel].flow_control = 0;
			break;
		default:
			return;
			break;
	}

	/* Set baud rate */
	switch(tty_get_baud_rate(tty)) {
	case 75:
		csr |= TX_CLK_75 | RX_CLK_75;
		break;
	case 110:
		csr |= TX_CLK_110 | RX_CLK_110;
		break;
	case 150:
		csr |= TX_CLK_150 | RX_CLK_150;
		break;
	case 300:
		csr |= TX_CLK_300 | RX_CLK_300;
		break;
	case 600:
		csr |= TX_CLK_600 | RX_CLK_600;
		break;
	case 1200: 
		csr |= TX_CLK_1200 | RX_CLK_1200; 
		break;
	case 1800: 
		csr |= TX_CLK_1800 | RX_CLK_1800;
		break;
	case 2000: 
		csr |= TX_CLK_2000 | RX_CLK_2000; 
		break;
	case 2400:
		csr |= TX_CLK_2400 | RX_CLK_2400;
		break;
	case 4800: 
		csr |= TX_CLK_4800  | RX_CLK_4800;
		break;
	case 9600: 
		csr |= TX_CLK_9600  | RX_CLK_9600;
		break;
	case 19200: 
		csr |= TX_CLK_19200 | RX_CLK_19200;
	break;
	case 38400:
		csr |= TX_CLK_38400 | RX_CLK_38400; 
		break;
	default:
		printk(KERN_ERR PFX "Slot [%d:%d] Channel %c : Illegal baud rate value: %d !\n",
				     	ipoctal->slot_id->carrier_number, 
					ipoctal->slot_id->slot_position,
					'a'+channel, 
					tty_get_baud_rate(tty));
		return;
	}

 	/* Set ignore break status */
 	imr = ipoctal_read_io_reg(
 			ipoctal,
 			&ipoctal->block_regs[block].u.w.imr);
 	
 	if((channel % 2) == 1)
 		imr_brk = IMR_DELTA_BREAK_B;
 	else
 		imr_brk = IMR_DELTA_BREAK_A;
 	
 	if (I_IGNBRK(tty))
 		imr &= ~imr_brk;
 	else
 		imr |= imr_brk;
 	
 	ipoctal_write_io_reg(
 		ipoctal,
 		&ipoctal->block_regs[block].u.w.imr, imr);
 
	mr1 |= MR1_ERROR_CHAR;
	mr1 |= MR1_RxINT_RxRDY;

	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.mr, mr1);
	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.mr, mr2);
	ipoctal_write_io_reg(ipoctal, &ipoctal->chan_regs[channel].u.w.csr, csr);

	ipoctal->chan_config[channel].baud = tty_get_baud_rate(tty);
	ipoctal->chan_config[channel].bits_per_char = cflag & CSIZE;
	ipoctal->chan_config[channel].parity = cflag & PARENB;
	ipoctal->chan_config[channel].stop_bits = cflag & CSTOPB;

}

inline void ipoctal_write_io_reg(struct ipoctal *ipoctal, unsigned char *dest, unsigned char value)
{
	unsigned long offset = 0;

	offset = ((void *) dest) - ipoctal->slot_id->io_space.address;
	ip_slot_write_uchar(value, ipoctal->slot_id, SLOT_IO_SPACE, offset);
}

inline void ipoctal_write_cr_cmd(struct ipoctal *ipoctal, unsigned char *dest, unsigned char value)
{
	ipoctal_write_io_reg(ipoctal, dest, value);
	udelay(DELAY_MICROSECONDS);
}

inline unsigned char ipoctal_read_io_reg(struct ipoctal *ipoctal, unsigned char *src)
{
	unsigned long offset = 0;
	unsigned char value = 0xFF;

	offset = ((void *) src) - ipoctal->slot_id->io_space.address;
	ip_slot_read_uchar(&value, ipoctal->slot_id, SLOT_IO_SPACE, offset);
	return value;
}
