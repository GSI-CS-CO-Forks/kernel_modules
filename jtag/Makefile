CPU ?= L866
include ../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS = -g -I. -I../vmebridge/include
LDLIBS = -lvmebus.$(CPU)
LDFLAGS = -L../vmebridge/lib
ALL  = jtag.$(CPU).o jtag.$(CPU)
SRCS = jtag.c lenval.c micro.c ports.c smemio.c
HDRS = lenval.h micro.h ports.h

all: $(ALL)

# Run on Workstation only

jtag.$(CPU).o: $(SRCS) $(HDRS)

jtag.$(CPU): jtag.$(CPU).o

clean:
	$(RM) jtag.$(CPU)
	$(RM) jtag.$(CPU).o

cleanall:
	$(RM) jtag.{$(ALL_CPUS_COMMAS)}
	$(RM) jtag.{$(ALL_CPUS_COMMAS)}.o
