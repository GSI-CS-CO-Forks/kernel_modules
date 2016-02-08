# include the build environment
include ../../../common.mk

#include version information
include ../../versions.mk

COMPILE_TIME:=$(shell date +%s)

CFLAGS= -g -Wall
#CFLAGS+= -I../driver
CFLAGS+= -DCOMPILE_TIME=$(COMPILE_TIME)
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LDLIBS = -lm

ALL  = amptest.$(CPU) amptest.$(CPU).o

SRCS = amptest.c AmpCmds.c AmpOpen.c DoCmd.c GetAtoms.c Cmds.c

HDRS = Cmds.h

TEST = amptest.c AmpCmds.c AmpOpen.c DoCmd.c GetAtoms.c Cmds.c

all: $(ALL)

clean:
	$(RM) amptest.$(CPU)
	$(RM) amptest.$(CPU).o

cleanall:
	$(RM) amptest.{$(ALL_CPUS_COMMAS)}
	$(RM) amptest.{$(ALL_CPUS_COMMAS)}.o


# Run on Workstation only

amptest.$(CPU).o: $(TEST) $(HDRS) $(SRCS)

PROGS_LIST=\
	amptest.$(CPU) \
	amp_news \
	amptest.config \
	RefAmpDspBin0 \
	RefAmpDspBin1 \
	RefAmpDspBin2 \
	RefAmpStatus \

install: install_prog_global
