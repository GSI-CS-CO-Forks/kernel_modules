#ifndef MSATT_H
#define MSATT_H

/* Vendor and device ID's for Magenta Sys Attenuator */
#define MSATT_VENDOR_ID		0x10DC
#define MSATT_DEVICE_ID		0x0190
#define MSATT_SUBVENDOR_ID	0xFFFF /* PCI_ANY_ID */
#define MSATT_SUBDEVICE_ID	0xFFFF /* PCI_ANY_ID */

/* BARs base address registers */
#define MSATT_CONFIG_MMIO_BAR	0
#define MSATT_REGS_BAR		1
#define MSATT_REGS_BAR_SIZE	4096

/* offset in BAR1 */
#define MSATT_DESCRIPTION_REG	0x000	/* read only */
#define MSATT_COMMAND_REG	0x010	/* read / write */

/* Version and description magic numbers */
#define MSATT_VERSION_1		0x01	/* Each relay can be set individually */
#define MSATT_VERSION_2		0x02	/* Auto enable x1 when relay x2 is enabled */
#define MSATT_N2		0x0200	/* Two channels card */
#define MSATT_N4		0x0400	/* Four channels card */

/* IOCTL commands */
#define MAJOR_NUM 'u' /* chosen arbitrarily */
#define IOCTL_SET_CMD	_IOR(MAJOR_NUM, 0, struct msatt_params)
#define IOCTL_GET_CMD	_IOR(MAJOR_NUM, 1, struct msatt_params)
#define IOCTL_GET_NUMCHAN	_IOR(MAJOR_NUM, 2, unsigned int)
struct msatt_params {
	int channel;
	int value;
};
#endif /* MSATT_H */

