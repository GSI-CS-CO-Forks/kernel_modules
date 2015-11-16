# include the build environment
include ../../../common.mk

CFLAGS += -g -Wall -D_GNU_SOURCE

CFLAGS+= -I. -I../../include/ \
	-I../../lib \
	-I../../../include/

all: libcvorg_extest.$(CPU).a

libcvorg_extest.$(CPU).a: cmd_noskel.$(CPU).o extest.$(CPU).o
	$(AR) rv $@ $^

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
