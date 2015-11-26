#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libvmebus.h>

static char git_version[] = "git_version: " GIT_VERSION;

void print_version(void);

void usage(char *prog)
{

	fprintf(stderr,
		"usage: %s [-v vme_address][options]\n\n", prog);
	fprintf(stderr,
		"This tool can read/write at a specific VME address\n\n");
	fprintf(stderr,
		"Options\n");
	fprintf(stderr,
		"  -v 0x<NUM> : VME address to access\n");
	fprintf(stderr,
		"  -s 0x<NUM> : address offset to access in bytes (skip bytes))\n");
	fprintf(stderr,
		"  -a 0x<NUM> : address modifier for VME bus (default 0x09)\n");
	fprintf(stderr,
		"  -l <NUM>   : VME map length in bytes (default 512KiB)\n");
	fprintf(stderr,
		"  -w <NUM>   : value to write\n");
	fprintf(stderr,
		"  -d <NUM>   : data width in bits (default 32 bits)\n");
	fprintf(stderr,
		"  -D <NUM>   : access data width in bits. It defines\n"
		"               which data type use to access the memory\n"
		"               (default 32 bits)\n");
	fprintf(stderr,
		"  -n <NUM>   : number of incremental accesses to the memory\n");
	fprintf(stderr,
		"  -H         : hide the address\n");
	fprintf(stderr,
		"  -h         : print this help\n");
	fprintf(stderr,
		"  -V         : version\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"Data Width\n");
	fprintf(stderr,
		"The supported data widths are 8, 16 and 32 for the access\n"
		"data width; 16 and 32 for the data width\n");
	fprintf(stderr, "\n");
	fprintf(stderr,
		"Address Modifier\n");
	fprintf(stderr,
		"The supported address modifier are the following:\n");
	fprintf(stderr, "  0x09 A32 user data\n");
	fprintf(stderr, "  0x29 A16 user data\n");
	fprintf(stderr, "  0x39 A24 user data\n");
	fprintf(stderr, "\n");
	print_version();
	exit(1);
}

void print_version(void)
{
	fprintf(stderr, "%s\n", git_version);
	fprintf(stderr, "%s\n", libvmebus_version_s);
}

int is_invalid_width(int data_width)
{
	return (data_width != 8 &&
		data_width != 16 &&
		data_width != 32);
}

static void vme_mem_write(int width, volatile void *addr, uint32_t value)
{
	switch (width) {
	case 8:
		*(uint8_t *)addr = value;
		break;
	case 16:
		*(uint16_t *)addr = htons(value);
		break;
	case 32:
		*(uint32_t *)addr = htonl(value);
		break;
	}
}

static uint32_t vme_mem_read(int width, volatile void *addr)
{
	switch (width) {
	case 8:
		return *(uint8_t *)addr;
	case 16:
		return ntohs(*(uint16_t *)addr);
	case 32:
		return ntohl(*(uint32_t *)addr);
	}
	return -1;
}

int main(int argc, char *argv[])
{

	struct vme_mapping map;
	struct vme_mapping *mapp = &map;
	volatile void *ptr;
	unsigned int vmebase, am, data_width, access_width;
	unsigned int offset;
	unsigned int length;
	int i, count;
	int c;
	int write, offsets_on;
	int width;
	uint32_t word, datum;

	/* vme defaults */
	count = 1;
	am = VME_A32_USER_DATA_SCT;
	data_width = access_width = VME_D32;

	write = 0;
	offsets_on = 1;
	offset = 0;
	length = 0x80000;
	while ((c = getopt(argc, argv, "Hv:s:D:d:a:n:w:l:hV")) != -1) {
		switch (c) {
		case 'H':
			offsets_on = 0;
			break;
		case 's':
			offset = strtoul(optarg, NULL, 0);
			break;
		case 'v':
			vmebase = strtoul(optarg, NULL, 0);
			break;
		case 'd':
		case 'D':
			width = strtoul(optarg, NULL, 0);
			if (is_invalid_width(width)) {
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
		case 'V':
			print_version();
			exit(1);
		default:
			break;
		}
	}

	/* Prepare mmap description */
	memset(mapp, 0, sizeof(*mapp));
	mapp->am = 		am;
	mapp->data_width = 	(data_width == 8) ? 16 : data_width;
	mapp->sizel = 		length;
	mapp->vme_addrl =	vmebase;

	/* Do mmap */
	ptr = vme_map(mapp, 1);
	if (!ptr) {
		printf("could not map at 0x%08x\n", vmebase);
		exit(1);
	}

	fprintf(stderr, "vme 0x%08x kernel %p user %p\n",
			vmebase, mapp->kernel_va, mapp->user_va);
	for (i = 0; i < count; i++, offset += 4) {
		volatile void *addr = ptr + offset;
		if (!write) {
			datum = vme_mem_read(access_width, addr);
			if (offsets_on)
				printf("%08x: ", vmebase + offset);
			printf("%08x\n", datum);
		} else {
			vme_mem_write(access_width, addr, word);
		}
	}

	vme_unmap(mapp, 1);
	return 0;
}
