# include the build environment
include ../../common.mk

CFLAGS+= -I../include

all: libvmebus.$(CPU).a

libvmebus.$(CPU).a: libvmebus.$(CPU).o
	$(AR) rv $@ $^

libvmebus.$(CPU).o: libvmebus.c libvmebus.h

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
