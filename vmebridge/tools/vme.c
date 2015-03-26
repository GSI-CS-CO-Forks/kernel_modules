#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libvmebus.h>

static char usage_string[] =
	"usage: %s [-oh?] [-w word] [-v vme_address]\n"
	"[-s skip_bytes] [-d data_width] [-D effective_width]"
	"[-l map_length] "
	"[-a address_modifier] [-n word_count]\n";

void usage(char *prog)
{
	fprintf(stderr, usage_string, prog);
	exit(1);
}

int invalid(int data_width)
{
	return (data_width != 8 &&
		data_width != 16 &&
		data_width != 32);
}

int main(int argc, char *argv[])
{

	struct vme_mapping map;
	struct vme_mapping *mapp = &map;
	volatile void *ptr;
	unsigned int vmebase, am, data_width, access_width;
	unsigned int offset, skip_bytes;
	unsigned int length;
	int i, count;
	int c;
	int write, offsets_on;
	int width;
	uint32_t word;

	/* vme defaults */
	count = 1;
	am = VME_A32_USER_DATA_SCT;
	data_width = access_width = VME_D32;

	write = 0;
	offsets_on = 1;
	skip_bytes = 0;
	length = 0x80000;
	while ((c = getopt(argc, argv, "ov:s:D:d:a:n:w:l:")) != -1) {
		switch (c) {
		case 'o':
			offsets_on = 0;
			break;
		case 's':
			skip_bytes = strtoul(optarg, NULL, 0);
			break;
		case 'v':
			vmebase = strtoul(optarg, NULL, 0);
			break;
		case 'd':
		case 'D':
			width = strtoul(optarg, NULL, 0);
			if (invalid(width)) {
				fprintf(stderr, "invalid data width %d\n", width);
				exit(1);
			}
			if (c == 'd')
				data_width = width;
			else
				access_width = width;
			break;
		case 'a':
			am = strtoul(optarg, NULL, 0);
			break;
		case 'n':
			count = strtoul(optarg, NULL, 0);
			break;
		case 'l':
			length = strtoul(optarg, NULL, 0);
			break;
		case 'w':
			write = 1;
			word = strtoul(optarg, NULL, 0);
			break;
		case '?':
		case 'h':
			usage(argv[0]);
			break;
		default:
			break;
		}
	}

	memset(mapp, 0, sizeof(*mapp));

	mapp->am = 		am;
	mapp->data_width = 	(data_width == 8) ? 16 : data_width;
	mapp->sizel = 		length;
	mapp->vme_addrl =	vmebase;

	if ((ptr = vme_map(mapp, 1)) == NULL) {
		printf("could not map at 0x%08x\n", vmebase);
		exit(1);
	}

	fprintf(stderr, "vme 0x%08x kernel %p user %p\n",
			vmebase, mapp->kernel_va, mapp->user_va);
	offset = skip_bytes;
	for (i = 0; i < count; i++, offset += 4) {
		volatile void *addr = ptr + offset;
		if (!write) {
			uint32_t datum;

			switch (access_width) {
			case 8:
				datum = *(uint8_t *)addr;
				break;
			case 16:
				datum = ntohs(*(uint16_t *)addr);
				break;
			case 32:
				datum = ntohl(*(uint32_t *)addr);
				break;
			}
			if (offsets_on)
				printf("%08x: ", vmebase + offset);
			printf("%08x\n", datum);
		} else {
			switch (access_width) {
			case 8:
				*(uint8_t *)addr = word;
				break;
			case 16:
				*(uint16_t *)addr = htons(word);
				break;
			case 32:
				*(uint32_t *)addr = htonl(word);
				break;
			}
		}
	}

	vme_unmap(mapp, 1);
	return 0;
}
