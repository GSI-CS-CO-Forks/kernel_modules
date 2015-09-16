CPU?=L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS = -Wall -I../include $(EXTRACFLAGS)
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
