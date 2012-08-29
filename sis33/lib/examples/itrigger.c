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
static struct sis33_itrigger_setup	setup;
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



void print_menu () 
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
        printf("\n----> Threshold: 0x%03x", setup.threshold);
        printf("\n----> GTLE: ");
	setup.gtle ? printf("LE\n") : printf ("GT\n");

	
        printf("\n----> Pulse mode is: ");
	setup.pulse_mode ? printf("ON\n") : printf("OFF\n");
        printf("----> P = 0x%x\n", setup.p);

        printf("\n----> N M mode is: ");
	setup.n_m_mode ? printf("ON\n") : printf("OFF\n");
        printf("----> N = 0x%x\n", setup.n);
        printf("----> M = 0x%x\n", setup.m);

        printf("\n\n [0] - Change trigger enabled\n");
        printf("\n [1] - Change current channel\n");
        printf("\n [2] - Change threshold\n");
        printf("\n [3] - Set GT mode\n");
        printf("\n [4] - Set LE mode\n");
        printf("\n [5] - Set/Unset pulse mode\n");
        printf("\n [6] - Change P value\n");
        printf("\n [7] - Set/Unset N M mode\n");
        printf("\n [8] - Change N value\n");
        printf("\n [9] - Change M value\n");
        printf("\n [q] - Quit test\n");
        printf("\n Choose an option: ");
}

int main(int argc, char *argv[])
{
	sis33_t *dev;
	char option;
	int ret;
	char cval[255];

	parse_args(argc, argv);

	/* log error messages */
	sis33_loglevel(3);

	dev = sis33_open(module_nr);
	if (dev == NULL)
		exit(EXIT_FAILURE);


	do {
		ret = sis33_get_internal_trigger_setup(dev, channel, &setup);
		if (ret < 0) {
			printf("Error reading internal trigger setup for LUN %d and channel 0\n", module_nr);
			goto exit;
		}

		sis33_get_enable_internal_trigger(dev, channel, &enabled);
		if (ret < 0) {
			printf("Error reading internal trigger status for LUN %d and channel 0\n", module_nr);
			goto exit;
		}

		print_menu();
		scanf("%c", &option);

		switch (option) {
		case '0':
			if (enabled != 0) 
				enabled = 0;
			else enabled = 1;
			sis33_set_enable_internal_trigger(dev, channel, enabled);
			break;
		case '1': 
			change_channel();
			ret = sis33_get_internal_trigger_setup(dev, channel, &setup);
			break;
		case '2':
			get_str("Enter new threshold (12 bits)", &cval[0]);
			setup.threshold = atoi(cval);	
			break;
		case '3':
			setup.gtle = 0;	
			break;
		case '4':
			setup.gtle = 1;	
			break;
		case '5':
			setup.pulse_mode = !setup.pulse_mode;	
			break;
		case '6':
			get_str("Enter new P value (4 bits)", &cval[0]);
			setup.p = atoi(cval);	
			break;
		case '7':
			setup.n_m_mode = !setup.n_m_mode;	
			break;
		case '8':
			 get_str("Enter new N value (4 bits)", &cval[0]);
			setup.n = atoi(cval);	
			break;
		case '9':
			get_str("Enter new M value (4 bits)", &cval[0]);
			setup.m = atoi(cval);	
			break;
		}

		sis33_set_internal_trigger_setup(dev, channel, setup);
	
	} while ((option != 'q') && (option != 'Q'));
 exit:
	if (sis33_close(dev))
		exit(EXIT_FAILURE);

	return 0;
}
