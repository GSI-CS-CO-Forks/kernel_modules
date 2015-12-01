#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#ifdef ppc4
	#include "libvmod16a22dioaio.h"
#else
	#include "libvmod16a2.h"
#endif

static char git_version[] = "git_version: " GIT_VERSION;

/* VMOD 16A2 precision */
#define VMOD_DAC_INPUT_RANGE (1<<16)

int volts2digital(float volts)
{
        unsigned int digital = VMOD_DAC_INPUT_RANGE*(volts+10.0)/20.0;

        if (digital >= VMOD_DAC_INPUT_RANGE)
                return -ERANGE;
        else
                return digital;
}

void usage(void)
{
	fprintf(stderr, "usage: pp lun channel volts\n\n");
	fprintf(stderr, "%s\n", git_version);
	fprintf(stderr, "%s\n", libvmod_version_s);
}

int main(int argc, char *argv[])
{
	int lun, channel, digital;
	float volts;
	int fd, err;

	if (argc != 4) {
		usage();
		return 1;
	}
	lun     = atoi(argv[1]);
	channel = atoi(argv[2]);
        volts   = atof(argv[3]);
	digital = volts2digital(volts);
	if (digital < 0) {
		usage();
		return 1;
	}

	if ((fd = vmod16a2_get_handle(lun)) < 0) {
		printf("could not open lun %d\n", lun);
		return 1;
	}

	printf("writing %6.3f volts (digital 0x%04x = %d)"
		"to channel %d \n",
		volts, digital, digital, channel);
	if ((err = vmod16a2_convert(fd, channel, digital)) != 0) {
		fprintf(stderr, "vmod62a2_convert exit status %d\n", err);
		return 1;
	}
	return 0;
}

