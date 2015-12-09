# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

# Flags to compile test program and lib
CFLAGS=-Wall -g
CFLAGS+=-I../lib
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS+=-L../lib
LDLIBS+=-lpciioconfig.$(CPU)

all: test_pciioconfig.$(CPU)

clean:
	$(RM) test_pciioconfig.$(CPU)

cleanall:
	$(RM) test_pciioconfig.{$(ALL_CPUS_COMMAS)}

PROGS_LIST=test_pciioconfig.$(CPU)

# use default instalation rule for programs/tools
install: install_prog_global
