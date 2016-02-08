# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS = -Wall -I../include $(EXTRACFLAGS)
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS = -L../lib
LDLIBS = -lvmebus.$(CPU)

progs := vme.$(CPU)

all: $(progs)

vme.$(CPU): vme.c

.PHONY: all clean celanall

clean:
	$(RM) $(progs) *.o

cleanall:
	$(RM) -r vme.{${ALL_CPUS_COMMAS}}

PROGS_LIST=vme.$(CPU)

# use default instalation rule for programs/tools
install: install_prog_global
