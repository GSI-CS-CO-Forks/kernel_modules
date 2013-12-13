#================================================================
# Makefile to produce standard CTR-P test program
#================================================================

CPU=L866

include /acc/dsc/src/co/Make.auto

CFLAGS= -g -Wall -I../include -I.
LDLIBS= -lm

ALL  =  ctrtest.$(CPU) \
	CtrReadInfo.$(CPU) \
	CtrWriteInfo.$(CPU) \
	CtrLookat.$(CPU) \
	CtrClock.$(CPU)

NOPSALL = CtrReadInfo.$(CPU) \
	CtrWriteInfo.$(CPU) \
	ctrtest.$(CPU) \

HDRS = Cmds.h

TEST = CtrCmds.c CtrOpen.c DoCmd.c GetAtoms.c Cmds.c
READ = CtrReadInfo.c CtrOpen.c
WRIT = CtrWriteInfo.c CtrOpen.c
LOOK = CtrLookat.c DisplayLine.c CtrOpen.c
CLOK = CtrClock.c CtrOpen.c

all: $(ALL)
all: CFLAGS += -DPS_VER -I/acc/local/$(CPU)/include/
all: LDLIBS += -ltgm -lerr -lerr_dummy -lX11
nops: $(NOPSALL)

# Run on Workstation only

ctrtest.$(CPU).o: ctrtest.c $(TEST) $(HDRS)
CtrReadInfo.$(CPU).o: $(READ) $(HDRS)
CtrWriteInfo.$(CPU).o: $(WRIT) $(HDRS)
CtrClock.$(CPU).o: $(CLOK) $(HDRS)
CtrLookat.$(CPU).o: $(LOOK) $(HDRS)

clean:
	$(RM) $(ALL) $(BAKS) *.o

install:
	echo "Please use install-lab/oper/lhc"

install-lab: ctrptest.$(CPU) ctrtest.$(CPU)
	dsc_install $(ALL) /acc/dsc/lab/$(CPU)/ctr

install-oper: ctrptest.$(CPU) ctrtest.$(CPU)
	dsc_install $(ALL) /acc/dsc/oper/$(CPU)/ctr


install-lhc: ctrptest.$(CPU) ctrtest.$(CPU)
	dsc_install $(ALL) /acc/dsc/oplhc/$(CPU)/ctr
