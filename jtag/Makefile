# include the build environment
include ../common.mk

CFLAGS = -g -I../vmebridge/include
LDLIBS = -lvmebus.$(CPU)
LDFLAGS = -L../vmebridge/lib
ALL  = jtag.$(CPU)
SRCS = jtag.c lenval.c micro.c ports.c smemio.c
HDRS = lenval.h micro.h ports.h

.PHONY: all clean cleanall

all: $(ALL)

jtag.$(CPU): $(SRCS:.c=.$(CPU).o) ../vmebridge/lib/libvmebus.$(CPU).a

../vmebridge/lib/libvmebus.$(CPU).a:
	$(MAKE) -C ../vmebridge/lib

vmebus_clean:
	$(MAKE) -C ../vmebridge/lib clean

vmebus_cleanall:
	$(MAKE) -C ../vmebridge/lib cleanall

clean: vmebus_clean
	$(RM) *.$(CPU)
	$(RM) *.$(CPU).o

cleanall: vmebus_cleanall
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
