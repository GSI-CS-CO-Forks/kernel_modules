#================================================================
# Makefile to produce standard CTR-P test program
#================================================================

# include the build environment
include ../../common.mk

DDIR = acdx

COMPILE_TIME:=$(shell date +%s)

ACCS=oplhc

CFLAGS= -g -Wall
CFLAGS+= -I../driver
CFLAGS+= -DCOMPILE_TIME=$(COMPILE_TIME)

LDLIBS = -lm

ALL  = acdxtest.$(CPU) acdxtest.$(CPU).o

SRCS = acdxtest.c AcdxCmds.c AcdxOpen.c DoCmd.c GetAtoms.c Cmds.c

HDRS = Cmds.h

TEST = acdxtest.c AcdxCmds.c AcdxOpen.c DoCmd.c GetAtoms.c Cmds.c

all: $(ALL)

clean:
	$(RM) acdxtest.$(CPU)
	$(RM) acdxtest.$(CPU).o

cleanall:
	$(RM) acdxtest.{$(ALL_CPUS_COMMAS)}
	$(RM) acdxtest.{$(ALL_CPUS_COMMAS)}.o


# Run on Workstation only

acdxtest.$(CPU).o: $(TEST) $(HDRS)

install: acdxtest.$(CPU)

