/*
 * version.c
 *
 * print the version of the library
 */

#include <stdio.h>

#include "libsis33.h"
#include "my_stringify.h"

static char git_version[] = "git_version: " GIT_VERSION;

int main(int argc, char *argv[])
{
	printf("%s\n", libsis33_version ? libsis33_version : "Unknown");
	printf("%s\n", git_version);
	printf("%s\n", libsis33_version_s);
	return 0;
}
