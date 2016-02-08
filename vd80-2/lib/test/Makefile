#================================================================
# Makefile to produce standard CTX1 test applications
#================================================================

include ../../../common.mk

#include version information
include ../../versions.mk

COMPILE_TIME:=$(shell date +%s)

VD80LB = ..
DRIVER = $(VD80LB)/../driver


CFLAGS = -g -Wall -I $(VD80LB) -I $(DRIVER) -DCOMPILE_TIME=$(COMPILE_TIME)
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LDLIBS = $(VD80LB)/libvd80.$(CPU).a

ALL  = vd80test.$(CPU)
SRCS = vd80test.c vd80Cmds.c DoCmd.c GetAtoms.c Cmds.c
HDRS = Cmds.h $(VD80LB)/libvd80.h $(DRIVER)/vd80.h

all: $(ALL)

clean:
	$(RM) *.$(CPU).o
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) *.{$(ALL_CPUS_COMMAS)}

vd80test.$(CPU).o: $(SRCS) $(HDRS)

vd80test.$(CPU): vd80test.$(CPU).o

PROGS_LIST=vd80test.$(CPU)

install: install_prog_global
