# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: cvoratest.$(CPU).sh

clean:
	$(RM) cvoratest.$(CPU).sh

cleanall:
	$(RM) cvoratest.{$(ALL_CPUS_COMMAS)}.sh

# Script cvoratest.$(CPU).sh is used to let cvoratest.py to load proper
# shared library
cvoratest.$(CPU).sh:
	echo "#!/bin/sh" > $@
	echo "CPU=.$(CPU) LIBCVORA_PATH=../ ./cvoratest.py" >> $@
	chmod +x $@

PROGS_LIST=cvoratest.py

install: install_prog_global
