# include the build environment
include ../../common.mk

CFLAGS+= -I../include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: libvmebus.$(CPU).a

libvmebus.$(CPU).a: libvmebus.$(CPU).o
	$(AR) rv $@ $^

libvmebus.$(CPU).o: libvmebus.c libvmebus.h

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
