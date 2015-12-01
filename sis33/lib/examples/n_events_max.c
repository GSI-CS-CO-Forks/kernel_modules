/*
 * n_events_max.c
 *
 * Show the number of events of a device
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libsis33.h"
#include "my_stringify.h"

static char git_version[] = "git_version: " GIT_VERSION;

/* default module number (LUN) */
#define MODULE_NR	0

#define PROGNAME	"n_events_max"

static int		module_nr = MODULE_NR;
extern char *optarg;

static const char usage_string[] =
	"Show the number of events of a device\n"
	" " PROGNAME " [-h] [-h] [-m<LUN>]";

static const char commands_string[] =
	"options:\n"
	" -h = show this help text\n"
	" -m = Module number (default: " my_stringify(MODULE_NR) ")";

static void usage_complete(void)
{
	printf("%s\n", usage_string);
	printf("%s\n", commands_string);
}

static void print_version(void)
{
	printf("%s\n", git_version);
	printf("%s\n", libsis33_version_s);
}

static void parse_args(int argc, char *argv[])
{
	int c;

	for (;;) {
		c = getopt(argc, argv, "hvm:");
		if (c < 0)
			break;
		switch (c) {
		case 'h':
			usage_complete();
			exit(EXIT_SUCCESS);
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
		case 'm':
			module_nr = strtol(optarg, NULL, 0);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	unsigned int events_max;
	sis33_t *dev;

	parse_args(argc, argv);

	/* log error messages */
	sis33_loglevel(3);

	dev = sis33_open(module_nr);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't open sis33.%d\n", module_nr);
		exit(EXIT_FAILURE);
	}
	if (sis33_get_max_nr_events(dev, &events_max) < 0)
		exit(EXIT_FAILURE);
	printf("%u\n", events_max);
	if (sis33_close(dev)) {
		fprintf(stderr, "sis33_close failed\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
