/*-
 * Copyright (c) 2010 Samuel I. Gonsalvez <siglesia@cern.ch>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of copyright holders nor the names of its
*    contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h> 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#ifdef ppc4
	#include "libvmodttl2dioaio.h"
#else
	#include "libvmodttl.h"
#endif

int device;

/* Thread for incomming messages */
void * wait_for_msg(void *data)
{
        int             fd = (int)data;
        int             i;
        unsigned char   buf[2];
        int             n;
        int             dev;
        int             val;

        while(1) {
		
                n = vmodttl_read_device(fd, buf); /* sleep until message arrive */
                if(n <= 0) {
                        fprintf(stderr,"panic read ...\n");
                        //return((void *)1);
			continue;
                }
                /* interpret the received command */
                dev = buf[0];
                val = buf[1];
                switch(dev) {
                case 0:
                case 1:
                case 2:
                        //printf("Device: 0x%x val: 0x%x\n", dev, val);
			if(val != 0x1 && val != 0x2)
				fprintf(stderr, "Valor MAL : 0x%x Canal: %d\n", val, dev);
                        break;
                case 4:
                        printf("Device %d, timer signal arrived\n", dev);
                        break;
                default:
                        for(i=0; i<2; i++) {
                                printf("0x%x ", buf[i]);
                        }
                        printf("\n");
                        break;
                }
		buf[0] = 255;
		buf[1] = 0;
		
#if 0	
	vmodttl_pattern(device, 0, 0, ANY);
	vmodttl_pattern(device, 0, 1, ANY);
	vmodttl_pattern(device, 0, 2, ANY);
	vmodttl_pattern(device, 0, 3, ANY);
	vmodttl_pattern(device, 0, 4, ANY);
	vmodttl_pattern(device, 0, 5, ANY);
	vmodttl_pattern(device, 0, 6, ANY);
	vmodttl_pattern(device, 0, 7, ANY);
#endif


	}
        
}

int main (int argc, char *argv[])
{        
        //int tmp;
	//char txt;
	int i;
	struct vmodttl_config conf;
	int fd;
	pthread_t thread;


	printf("  ______________________________________  \n");
	printf(" |                                      | \n");
	printf(" |       VMOD-TTL Testing program       | \n");
	printf(" |______________________________________| \n");
	printf(" |                                      | \n");
	printf(" | Created by: Samuel I. Gonsalvez      | \n");
	printf(" | Email: siglesia@cern.ch              | \n");
	printf(" |______________________________________| \n");

	if (argc == 2)
	{
		device = atoi(argv[1]);
	}
	else
	{
		printf("\n Please use: test_read_write <lun_of_the_device>.\n");
		exit(-1);
	}

	if(vmodttl_open (device) < 0) {
		perror("open failed");	
		return -1;
	}

	printf("\n Port configuration \n");
	printf("\n Channel A [ (0) Input - (1) Output]: \n");
	//scanf("%d", &tmp);
	conf.dir_a = 0;
	if (!conf.dir_a){
		conf.mode_a = 0;
	}else{
		printf("\n Channel A [ (0) Open Drain - (1) Open Collector ]\n");
		//scanf("%d", &tmp);
		conf.mode_a = 0;
	}

	printf("\n Channel B [ (0) Input - (1) Output]: \n");
	//scanf("%d", &tmp);
	conf.dir_b = 1;

	if (!conf.dir_b){
		conf.mode_b = 0;

	}else{
		printf("\n Channel B [ (0) Open Drain - (1) Open Collector ]\n");
		//scanf("%d", &tmp);
		conf.mode_b = 0;
	}

	printf("\n Channel C [ (0) Open Drain - (1) Open Collector ]\n");
	//scanf("%d", &tmp);
	conf.mode_c = 0;
	printf("\n Up time of the pulse in the data strobe (useg): \n");
	//scanf("%d", &tmp);
	conf.us_pulse = 0;

	printf("\n In all the channels: [ (0) Non-inverting Logic - (1) Inverting logic]: \n");
	//scanf("%d", &tmp);
	conf.inverting_logic = 0;

	for(i = 0; i< NUM_BITS; i++) {
		conf.conf_pattern_a[i] = OFF;
		conf.conf_pattern_b[i] = OFF;
	}

	conf.pattern_mode_a = OR;
	conf.pattern_mode_b = OR;

	if (vmodttl_io_config(device, conf) < 0)
		exit(-1);


	vmodttl_pattern(device, 0, 0, ZERO_TO_ONE);
	vmodttl_pattern(device, 0, 1, ZERO_TO_ONE);
	vmodttl_pattern(device, 0, 2, OFF);
	vmodttl_pattern(device, 0, 3, OFF);
	vmodttl_pattern(device, 0, 4, OFF);
	vmodttl_pattern(device, 0, 5, OFF);
	vmodttl_pattern(device, 0, 6, OFF);
	vmodttl_pattern(device, 0, 7, OFF);

	if ((fd = vmodttl_open(device)) < 0){
		fprintf(stderr, "Error opening lun %d: %s\n", device, strerror(errno));
		return -1;
	}	
	pthread_create( &thread, NULL, wait_for_msg, (void *)device );

	//sleep(60);
	while(1){};
	
	close(fd);
	vmodttl_close(device); 

	return 0;
}
