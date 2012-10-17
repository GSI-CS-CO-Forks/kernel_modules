/*
 * itrigger.c
 *
 * configure and set internal triggers
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <libsis33.h>
#include "my_stringify.h"

#define PROGNAME	"itrigger"

/* default module number (LUN) */
#define MODULE_NR	0

static int			module_nr = MODULE_NR;
static int			channel = 0;
static int			enabled = 0;
extern char *optarg;

static const char usage_string[] =
	"configure and set internal triggers on an sis33 device.\n"
	" " PROGNAME " [-h] [-m<LUN>] ";

static const char commands_string[] =
	"options:\n"
	" -h = show this help text\n"
	" -m = Module number (default: " my_stringify(MODULE_NR) ")\n";

static void usage_complete(void)
{
	printf("%s\n", usage_string);
	printf("%s\n", commands_string);
}

static void parse_args(int argc, char *argv[])
{
	int c;

	for (;;) {
		c = getopt(argc, argv, "e:hm:s:");
		if (c < 0)
			break;
		switch (c) {
		case 'h':
			usage_complete();
			exit(EXIT_SUCCESS);
		case 'm':
			module_nr = strtol(optarg, NULL, 0);
			break;
		}
	}
}

void get_str (char *text, char *str)
{
        char ch;

        /* Manual flush ?? */
        while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}
	
	printf("\n%s", text);
        scanf("%s",str);
}

void change_channel ()
{
        char option[255];
	int chan;

	get_str("Enter new channel [0-7]: ",&option[0]);
	chan = atoi(option);
        if ((chan >= 0) && (chan <= 8)) {
                channel = chan;
        } else {
                printf("Invalid channel\n");
        }
}

struct sis33_setup {
	int pulse_length;
	int sum_g;
	int peaking_time;
};



void print_menu (struct sis33_setup *setup, int threshold, int direction, enum sis3320_trigger_mode tmode) 
{
        /*printf("\033[2J"); */       /*  clear the screen  */
        /*printf("\033[H"); */        /*  position cursor at top-left corner */

        printf("\n--------------------------------------\n");
        printf("|        sis3320 internal trigger      |\n");
        printf("|            Test program            |\n");
        printf("--------------------------------------\n");

        printf("\n----> Internal trigger is: ");
	(enabled != 0) ? printf("ENABLED\n\n") : printf ("DISABLED\n\n");

        printf("\n----> Selected channel is %d\n", channel);
        printf("\n----> Threshold: 0x%03x", threshold);
        printf("\n----> GTLT: ");
	(direction > 0) ? printf("LT\n") : printf ("GT\n");
	printf("\n----> Trigger mode:");
	(tmode == SIS3320_FIR) ? printf("FIR\n") : printf ("Leading Edge\n");

	
        printf("\n----> Pulse length is 0x%x\n", setup->pulse_length);
        printf("\n----> SumG is 0x%x\n", setup->sum_g);
        printf("\n----> Peaking time is 0x%x\n", setup->peaking_time);

        printf("\n [0] - Enable/Disable itrigger\n");
        printf("\n [1] - Change current channel\n");
        printf("\n [2] - Change threshold\n");
        printf("\n [3] - Set GT/GE mode\n");
        printf("\n [4] - Set LT mode\n");
        printf("\n [5] - Set/Unset FIR mode\n");
        printf("\n [6] - Change pulse length value\n");
        printf("\n [7] - Change sumG value\n");
        printf("\n [8] - Change peaking time value\n");
        printf("\n [r] - Reset all\n");
        printf("\n [q] - Quit test\n");
        printf("\n Choose an option: ");
}

int get_setup (sis33_t *dev, struct sis33_setup *setup)
{
	int ret = 0;

	ret = sis33_get_internal_trigger_setup(dev, channel, SIS3320_PULSE_LENGTH, &setup->pulse_length);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3320_SUM_G, &setup->sum_g);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3320_PEAKING_TIME, &setup->peaking_time);

	return ret;

}

int main(int argc, char *argv[])
{
	sis33_t *dev;
	char option;
	int ret;
	char cval[255];
	int direction=0;
	int th;
	struct sis33_setup setup;
	enum sis3320_trigger_mode tmode;

	parse_args(argc, argv);

	/* log error messages */
	sis33_loglevel(4);

	dev = sis33_open(module_nr);
	if (dev == NULL)
		exit(EXIT_FAILURE);

	do {
		ret = get_setup(dev, &setup);
		if (ret < 0) {
			printf("Error reading internal trigger setup for LUN %d and channel 0\n", module_nr);
			goto exit;
		}

		ret = sis33_get_internal_trigger_threshold(dev, channel, &th);
		ret += sis33_get_internal_trigger_enable(dev, &enabled);
		ret += sis33_get_internal_trigger_direction(dev, channel, &direction);
		ret += sis33_get_internal_trigger_mode(dev, channel, &tmode);
		if (ret < 0) {
			printf("Error reading internal trigger status for LUN %d and channel 0\n", module_nr);
			goto exit;
		}

		print_menu(&setup, th, direction, tmode);
		scanf("%c", &option);

		switch (option) {
		case '0':
			enabled = (enabled == 0);
			ret = sis33_set_internal_trigger_enable(dev, enabled);
			break;
		case '1': 
			change_channel();
			ret = get_setup(dev, &setup);
			break;
		case '2':
			get_str("Enter new threshold (12 bits)", &cval[0]);
			th = atoi(cval);	
			ret = sis33_set_internal_trigger_threshold(dev, channel, th);
			break;
		case '3':
			direction = 0;	
			ret = sis33_set_internal_trigger_direction(dev, channel, direction);
			break;
		case '4':
			direction = 1;	
			ret = sis33_set_internal_trigger_direction(dev, channel, direction);
			break;
		case '5':
			if (tmode == SIS3320_FIR)
				tmode = SIS3320_LEADING_EDGE;
			else
				tmode = SIS3320_FIR;	
			ret = sis33_set_internal_trigger_mode(dev, channel, tmode); 
			break;
		case '6':
			get_str("Enter new Pulse value (8 bits)", &cval[0]);
			setup.pulse_length = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, 
					SIS3320_PULSE_LENGTH, setup.pulse_length);
			break;
		case '7':
			get_str("Enter new sumG value (5 bits)", &cval[0]);
			setup.sum_g = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, SIS3320_SUM_G, setup.sum_g);
			break;
		case '8':
			get_str("Enter new peaking time value (5 bits)", &cval[0]);
			setup.peaking_time = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, 
					SIS3320_PEAKING_TIME, setup.peaking_time);
			break;
		case 'r':
			ret = sis33_reset_internal_trigger_all(dev);
			break;
		}

		if (ret < 0) {
			printf("Error\n");
			goto exit;
		}

	
	} while ((option != 'q') && (option != 'Q'));
 exit:
	if (sis33_close(dev))
		exit(EXIT_FAILURE);

	return 0;
}
