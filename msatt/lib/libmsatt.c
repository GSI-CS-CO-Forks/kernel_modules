/**
 *	@file   libmsatt.c
 * 	@brief  Library file for the msatt driver
 * 	@author Luis Fernando Ruiz Gago
 *	@date	June 14th 2012
 */


#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "msatt.h"
#include "libmsatt.h"

const char * const libmsatt_version_s = "libmsatt version: " GIT_VERSION;

int msatt_open(int lun)
{
	const int filename_sz = 256;
	char devname[filename_sz];
	const char *driver_name = "msatt";

	int fd;

	/* open the device file */
	snprintf(devname, filename_sz, "/dev/%s.%d", driver_name, lun);
	printf("Trying to open device %s\n", devname);
	fd = open(devname, O_RDONLY);

	return fd;
}

int msatt_close (int fd)
{
	return close(fd);
}

enum msatt_value_enum msatt_get_relay (int fd, int channel)
{
	struct msatt_params arg;
	
	/* Client request: channels are 1,2,3,4 */
	if (channel < 1 || channel > 4)
		return ATT_ERR;
	
	arg.channel = channel; 
	arg.value = 0; /* just in case */

	
	if (ioctl(fd, IOCTL_GET_CMD, &arg) < 0)
	{
		return -1; 
	}

	return arg.value;
}

int msatt_set_relay (int fd, int channel, enum msatt_value_enum val)
{
	struct msatt_params arg;

	/* Client request: channels are 1,2,3,4 */
	if (channel < 1 || channel > 4)
		return ATT_ERR;

	arg.channel = channel;
	arg.value = val;
	
	if ((val != ATT_NONE) && (val != ATT_1)	&& (val != ATT_2))
		return ATT_ERR;
	
	return ioctl(fd, IOCTL_SET_CMD, &arg);
}

int msatt_get_nchannels(int fd)
{
	unsigned int nchan = 0;

	if (ioctl(fd, IOCTL_GET_NUMCHAN, &nchan) >= 0) {
		return nchan;
	}

	return -1;
}
