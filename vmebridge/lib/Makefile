CPU?=L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS+= -I../include

all: libvmebus.$(CPU).a

libvmebus.$(CPU).a: libvmebus.$(CPU).o
	$(AR) rv $@ $^

libvmebus.$(CPU).o: libvmebus.c libvmebus.h

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
