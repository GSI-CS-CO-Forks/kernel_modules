CPU ?= L866
include ../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS = -g -I. -I../vmebridge/include
LDLIBS = -lvmebus.$(CPU)
LDFLAGS = -L../vmebridge/lib
ALL  = jtag.$(CPU)
SRCS = jtag.c lenval.c micro.c ports.c smemio.c
HDRS = lenval.h micro.h ports.h

.PHONY: all clean cleanall

all: $(ALL)

# Run on Workstation only

jtag.$(CPU).o: $(SRCS) $(HDRS)

jtag.$(CPU): jtag.$(CPU).o ../vmebridge/lib/libvmebus.$(CPU).a

../vmebridge/lib/libvmebus.$(CPU).a:
	$(MAKE) -C ../vmebridge/lib

vmebus_clean:
	$(MAKE) -C ../vmebridge/lib clean

vmebus_cleanall:
	$(MAKE) -C ../vmebridge/lib cleanall

clean: vmebus_clean
	$(RM) jtag.$(CPU)
	$(RM) jtag.$(CPU).o

cleanall: vmebus_cleanall
	$(RM) jtag.{$(ALL_CPUS_COMMAS)}
	$(RM) jtag.{$(ALL_CPUS_COMMAS)}.o
