#include        <string.h> 
#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include        <sys/ioctl.h>
#include        <sys/types.h>
#include        <stdlib.h>
#include        <unistd.h>

#ifdef ppc4
	#include "libvmoddor2dioaio.h"
#else
	#include "libvmoddor.h"
#endif

static char git_version[] = "git_version: " GIT_VERSION;

int main (int argc, char *argv[])
{
	int lun = -1;
	int ret = -1;
	struct vmoddor_warg val;

	if(argc != 5){
		printf("tstlibdor <lun> <offset> <size> <value>\n");
		printf("<lun>		Logical Unit Number.\n"
			"<offset>	value of the offset (first channel) applied.\n"
			"<size>		size of the data (4, 8, 16 channels).\n"
			"<value>	integer value to be written (dec, hex and oct accepted).\n");
		printf("\n%s\n", git_version);
		printf("%s\n", libvmoddor_version_s);
		exit(-1);
	} 
	else
	{
		lun = strtoul(argv[1], NULL, 0);
		val.offset = strtoul(argv[2], NULL, 0);
		val.size = strtoul(argv[3], NULL, 0);
		val.data = strtol(argv[4], NULL, 0);
	}

	ret = vmoddor_open(lun);
	if(ret < 0){
		perror("open failed");
		exit(-1);
	}

	ret = vmoddor_write(lun, val);
	if (ret <0){
		perror("Error on put data to channel");
		return -1;
	}
	
	return 0;
}
