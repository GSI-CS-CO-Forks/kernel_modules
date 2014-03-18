#================================================================
# Makefile to produce TimLib test program
#================================================================

CPU=L866

DRVR = ../../driver/include

include /acc/dsc/src/co/Make.auto

CFLAGS = -DCTR_PCI -g -Wall -I. -I.. -I$(DRVR) -I/acc/local/$(CPU)/include

LDLIBS = ../libtim.$(CPU).a -ltgm -lerr -lerr_dummy -ltgv -lm -lX11 -ldtm -lnc

ALL  = Main.$(CPU) timtest.$(CPU) TimLookat.$(CPU) TimClock.$(CPU)

SRCS = Main.c TimCmds.c DoCmd.c GetAtoms.c Cmds.c

HDRS = Cmds.h ../TimLib.h

all: $(ALL)

clean:
	$(RM) *.$(CPU) *.$(CPU).o ,*

# Run on Workstation only

TimLookat.$(CPU).o: TimLookat.c

TimLookat.$(CPU): TimLookat.$(CPU).o

TimClock.$(CPU).o: TimClock.c

TimClock.$(CPU): TimClock.$(CPU).o

timtest.$(CPU): Main.$(CPU)
	rm -f timtest.$(CPU)
	cp Main.$(CPU) timtest.$(CPU)

Main.$(CPU).o: $(SRCS) $(HDRS)

Main.$(CPU): Main.$(CPU).o

install-lab: 
	d=/acc/dsc/lab/$(CPU)/tim; \
	echo Installing timtest in $$d;\
	dsc_install timtest.$(CPU) $$d;\
	echo Installing configuration file; \
	dsc_install timtest.config $$d;\
	echo Installing TimLookat.$(CPU) in $$d; \
	dsc_install TimLookat.$(CPU) $$d; \
	echo Installing TimClock.$(CPU) in $$d; \
	dsc_install TimClock.$(CPU) $$d; \
