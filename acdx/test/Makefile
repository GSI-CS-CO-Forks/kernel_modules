#================================================================
# Makefile to produce standard CTR-P test program
#================================================================

# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

COMPILE_TIME:=$(shell date +%s)

CFLAGS= -g -Wall
CFLAGS+= -I../driver
CFLAGS+= -DCOMPILE_TIME=$(COMPILE_TIME)
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LDLIBS = -lm

ALL  = acdxtest.$(CPU) acdxtest.$(CPU).o

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

PROGS_LIST=\
	acdxtest.$(CPU) \
	aqn.acdx \
	Acdx.bit \
	AcdxConfig \
	AcdxFreqGain \
	AcdxLowPass \
	AcdxMacro \
	AcdxPlotAqn \
	AcdxPlotFun \
	acdxtest.config \
	AmplifierOneCurrent \
	AmplifierOneVoltage \
	AmplifierTwoCurrent \
	AmplifierTwoVoltage \
	MagnetCurrent \
	MagnetVoltage \
	x3_servo_v2.9.bit \
	x3_servo_v2.10.bit \
	x3_servo_v2.11.bit \
	x3_servo_v2.12.bit \
	x3_servo_v2.13.bit \
	x3_servo_v2.14.bit \

install: install_prog_global
