#include <errno.h>
#include <string.h> 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#include "msatt.h"
#include "libmsatt.h"

int device;
int selected_channel;
int relay_register;
int num_channels;

char relay_bit_string[20];

void uint16_to_bit_string(char *s, uint16_t r)
{
	uint16_t bitmask;
	int i;

	bitmask = (1 << 15);
	for (i = 0; i < 19; i++) {
		s[i] = ' ';
		if (i % 5 == 4)
			continue;
		s[i] = r & bitmask ? '1' : '0';
		bitmask >>= 1;
	}
	s[i] = 0;
}

int get_dB_at_channel (int chan)
{
	int chan_db;

	chan_db = 3 << ((chan -1) * 4);
	chan_db = (chan_db & relay_register) >> ((chan -1) * 4);
	if (chan_db == 1)
		chan_db = 20;
	else if (chan_db == 3)
		chan_db = 40;

	return chan_db;
}

void print_menu () 
{
	int i;

	uint16_to_bit_string(relay_bit_string, relay_register);

	printf("\033[2J");        /*  clear the screen  */
	printf("\033[H");         /*  position cursor at top-left corner */

	printf("\n--------------------------------------\n");
	printf("|        Magenta Sys Attenuator      |\n");
	printf("|            Test program            |\n");
	printf("--------------------------------------\n");

	printf("\n----> Selected channel is %d\n", selected_channel);
	printf("\n----> Relays command register: %08x = [%s]\n",
		relay_register, relay_bit_string);

	for (i=1; i<=num_channels;i++) {
		printf("\n\tChannel %d: %2d dB ", i, get_dB_at_channel(i));
		if (i == selected_channel)
			printf("[*]");
	}

	printf("\n\n [0] - Re-read attenuation values\n");
	printf("\n\n [1] - Change current channel\n");
	printf("\n [2] - Set attenuation to 20dB\n");
	printf("\n [3] - Set attenuation to 40dB\n");
	printf("\n [4] - Unset attenuation\n");
	printf("\n [q] - Quit test\n");
	printf("\n Choose an option: ");
}

int get_register_value (int fd)
{
	int value = 0;
	int tmp;
	int i;

	for (i=1; i<=num_channels; i++) {
		tmp = msatt_get_relay(fd, i);
		tmp = tmp << 4 * (i-1);
		value |= tmp;
	}
	return value;
}

void change_channel ()
{
	char option;
	char ch;

	/* Manual flush ?? */
	while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}

	printf("\nEnter new channel [1-4]: ");
	scanf("%c",&option);
	if ((option >= '1') && (option <= '4')) {
		selected_channel = atoi(&option);	
	} else {
		printf("Invalid channel\n");
	}
}

int main (int argc, char *argv[])
{
	int fd;
	char option;

	selected_channel = 1;

	if (argc == 2)
	{
		device = atoi(argv[1]);
	}
	else
	{
		printf("Insufficient number of arguments\n");
		printf("\nPlease use: %s <lun_of_the_device>\n", argv[0]);
		exit(-1);
	}	

	if ((fd = msatt_open(device)) < 0) {
		perror ("open failed\n");
		return -1;
	}	

	/* Check for channel count*/
	num_channels = msatt_get_nchannels(fd);

	if (num_channels < 0) {
		printf("Can not get channel count. Exiting...\n");
		return -1;
	}

	/* First time, get value */
	relay_register = get_register_value(fd);

	do {
		relay_register = get_register_value(fd);
		print_menu();
		scanf("%c", &option);
		
		switch (option) {
		case '0':
			relay_register = get_register_value(fd);
			break;
		case '1':
			change_channel();
			break;
		case '2':
			msatt_set_relay(fd, selected_channel, ATT_1);
			relay_register = get_register_value(fd);
			break;
		case '3':
			msatt_set_relay(fd, selected_channel, ATT_2);
			relay_register = get_register_value(fd);
			break;
		case '4':
			msatt_set_relay(fd, selected_channel, ATT_NONE);
			relay_register = get_register_value(fd);
			break;
		default:
			if ((option != 'q') && (option != 'Q'))
				printf("Unknown option\n");
			break;
		}
	} while ((option != 'q') && (option != 'Q'));

	/* That's all folks! */
	msatt_close(fd);

	return 1;

}
