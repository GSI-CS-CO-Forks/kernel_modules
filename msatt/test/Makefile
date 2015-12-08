# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS=-g -Wall
CFLAGS+=-I../lib -I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS += -L../lib
LDLIBS += -lmsatt.$(CPU)

all: msattntst.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}


PROGS_LIST=msattntst.$(CPU)

# use default instalation rule for programs/tools
install: install_prog_global
