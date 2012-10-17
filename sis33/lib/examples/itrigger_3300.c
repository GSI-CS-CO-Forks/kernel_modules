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
	int pulse_mode;
	int p;
	int n_m_mode;
	int n;
	int m;
};



void print_menu (struct sis33_setup *setup, int threshold, int gtle) 
{
        printf("\033[2J");        /*  clear the screen  */
        printf("\033[H");         /*  position cursor at top-left corner */

        printf("\n--------------------------------------\n");
        printf("|        sis33 internal trigger      |\n");
        printf("|            Test program            |\n");
        printf("--------------------------------------\n");

        printf("\n----> Internal trigger is: ");
	(enabled != 0) ? printf("ENABLED\n\n") : printf ("DISABLED\n\n");

        printf("\n----> Selected channel is %d\n", channel);
        printf("\n----> Threshold: 0x%03x", threshold);
        printf("\n----> GTLE: ");
	(gtle > 0) ? printf("LE\n") : printf ("GT\n");

	
        printf("\n----> Pulse mode is: ");
	(setup->pulse_mode > 0) ? printf("ON\n") : printf("OFF\n");
        printf("----> P = 0x%x\n", setup->p);

        printf("\n----> N M mode is: ");
	setup->n_m_mode ? printf("ON\n") : printf("OFF\n");
        printf("----> N = 0x%x\n", setup->n);
        printf("----> M = 0x%x\n", setup->m);

        printf("\n [0] - Enable/Disable itrigger\n");
        printf("\n [1] - Change current channel\n");
        printf("\n [2] - Change threshold\n");
        printf("\n [3] - Set GT mode\n");
        printf("\n [4] - Set LE mode\n");
        printf("\n [5] - Set/Unset pulse mode\n");
        printf("\n [6] - Change P value\n");
        printf("\n [7] - Set/Unset N M mode\n");
        printf("\n [8] - Change N value\n");
        printf("\n [9] - Change M value\n");
        printf("\n [r] - Reset all\n");
        printf("\n [q] - Quit test\n");
        printf("\n Choose an option: ");
}

int get_setup (sis33_t *dev, struct sis33_setup *setup)
{
	int ret = 0;

	ret = sis33_get_internal_trigger_setup(dev, channel, SIS3300_PULSE_MODE, &setup->pulse_mode);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3300_P, &setup->p);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3300_N_M_MODE, &setup->n_m_mode);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3300_N, &setup->n);
	ret += sis33_get_internal_trigger_setup(dev, channel, SIS3300_M, &setup->m);

	return ret;

}

int main(int argc, char *argv[])
{
	sis33_t *dev;
	char option;
	int ret;
	char cval[255];
	int gtle=0;
	int th;
	struct sis33_setup setup;

	parse_args(argc, argv);

	/* log error messages */
	sis33_loglevel(3);

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
		ret += sis33_get_internal_trigger_direction(dev, channel, &gtle);
		if (ret < 0) {
			printf("Error reading internal trigger status for LUN %d and channel 0\n", module_nr);
			goto exit;
		}

		print_menu(&setup, th, gtle);
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
			gtle = 0;	
			ret = sis33_set_internal_trigger_direction(dev, channel, gtle);
			break;
		case '4':
			gtle = 1;	
			ret = sis33_set_internal_trigger_direction(dev, channel, gtle);
			break;
		case '5':
			setup.pulse_mode = (setup.pulse_mode == 0);	
			ret = sis33_set_internal_trigger_setup(dev, channel,
								 SIS3300_PULSE_MODE, setup.pulse_mode);
			break;
		case '6':
			get_str("Enter new P value (4 bits)", &cval[0]);
			setup.p = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, SIS3300_P, setup.p);
			break;
		case '7':
			setup.n_m_mode = (setup.n_m_mode == 0);	
			ret = sis33_set_internal_trigger_setup(dev, channel, 
								SIS3300_N_M_MODE, setup.n_m_mode);
			break;
		case '8':
			get_str("Enter new N value (4 bits)", &cval[0]);
			setup.n = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, SIS3300_N, setup.n);
			break;
		case '9':
			get_str("Enter new M value (4 bits)", &cval[0]);
			setup.m = atoi(cval);	
			ret = sis33_set_internal_trigger_setup(dev, channel, SIS3300_M, setup.m);
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
