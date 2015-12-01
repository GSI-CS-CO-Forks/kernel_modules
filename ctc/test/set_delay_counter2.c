#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <libctc.h>

static char git_version[] = "git_version: " GIT_VERSION;

/* doing two levels of stringification allows us to stringify a macro */
#define my_stringify_l(x...)	#x
#define my_stringify(x...)	my_stringify_l(x)


#define PROGNAME	"set_clk"
#define MODULE_NR 	0
#define CHANNEL_NR 	1
#define DELAY 	1

int fd;
int delay = DELAY;
int channel_nr = CHANNEL_NR;
int module_nr = MODULE_NR;

int delay_read = 0;

extern char *optarg;

static const char usage_string[] =
	"Setup delay counter 2 on a channel \n"
	" " PROGNAME " [-h] [-v] [-c<CHAN>] [-m<LUN>] [-s<CLK>]";

static const char commands_string[] =
	"options:\n"
	" -c = channel number (default: " my_stringify(CHANNEL_NR) ")\n"
	" -h = show this help text\n"
	" -m = Module number (default: " my_stringify(MODULE_NR) ")\n"
	" -s = Set delay counter 2(default: " my_stringify(DELAY) ")\n"
	" -v = version";

static void usage_complete(void)
{
	printf("%s\n", usage_string);
	printf("%s\n", commands_string);
}

static void print_version(void)
{
	printf("%s\n", git_version);
	printf("%s\n", libctc_version_s);
}

static void parse_args(int argc, char *argv[])
{
	int c;

	for (;;) {
		c = getopt(argc, argv, "c:hvm:s:");
		if (c < 0)
			break;
		switch (c) {
		case 'c':
			channel_nr = atoi(optarg);
			if (!channel_nr) {
				fprintf(stderr, "Invalid channel_nr\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			usage_complete();
			exit(EXIT_SUCCESS);
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
		case 'm':
			module_nr = atoi(optarg);
			break;
		case 's':
			delay = atoi(optarg);
			break;
		}
	}
}


int main(int argc, char *argv[])
{

	parse_args(argc, argv);
	
	fd = ctc_open(module_nr);	
		
	if (fd == 0) {
		fprintf(stderr, "Error opening the device: %s\n", strerror(errno));
		exit(-1);
	}

	// Read the setup before change it
	if(ctc_chan_get_delay_counter2(fd, channel_nr, &delay_read)) {
		fprintf(stderr, "Error get  delay counter2 from channel %d: %s\n", channel_nr, strerror(errno));
		goto out;
	}

	printf("Original -- Dev %d Channel %d delay counter 2 %d\n", module_nr, channel_nr, delay_read);


	printf("Writing -- Dev %d Channel %d delay counter 2 %d\n", module_nr, channel_nr, delay);
	// Write the setup 
	if(ctc_chan_set_delay_counter2(fd, channel_nr, delay)) {
		fprintf(stderr, "Error set %d  delay counter2 to channel %d: %s\n", delay, channel_nr, strerror(errno));
		goto out;
	}

	delay_read = 0;
	// Read again the setup
	if(ctc_chan_get_delay_counter2(fd, channel_nr, &delay_read)) {
		fprintf(stderr, "Error get  delay counter2 from channel %d: %s\n", channel_nr, strerror(errno));
		goto out;
	}

	printf("Read -- Dev %d Channel %d delay counter 2 %d\n", module_nr, channel_nr, delay_read);

out:
	ctc_close(fd);
	return 0;
}
