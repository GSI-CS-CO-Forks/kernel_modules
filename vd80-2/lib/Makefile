CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

CFLAGS= -g -Wall -I../driver -fPIC -DCOMPILE_TIME=$(shell date +%s)
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

SRCS = libvd80.c libvd80.h

all: libvd80.$(CPU).so libvd80.$(CPU).a

libvd80.$(CPU).o: $(SRCS)

libvd80.$(CPU).so: libvd80.$(CPU).o
	$(CC) $(CFLAGS) -o $@ -shared $^

libvd80.$(CPU).a: libvd80.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) *.$(CPU).{o,so,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
