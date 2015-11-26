# include the build environment
include ../../common.mk

CFLAGS += -g -Wall -D_GNU_SOURCE

CFLAGS+= -Iextest/ \
	-I../ad9516 \
	-I../lib \
	-I../include/ \
	-I../../include/
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LOADLIBES := -L../lib/ -L../ad9516
LDLIBS=-lcvorg.$(CPU) -lreadline -lad9516.$(CPU) -lrt -lm

#L865 requires tempcap for readline
ifeq ($(CPU),L865)
LDLIBS+=-ltermcap
endif

.PHONY: clean cleanall all

all: cvorgtest.$(CPU)

cvorgtest.$(CPU): cvorgtest.$(CPU).o extest/libcvorg_extest.$(CPU).a

extest/libcvorg_extest.$(CPU).a:
	$(MAKE) -C extest CPU=$(CPU)

clean:
	$(RM) *.$(CPU).o
	$(RM) *.$(CPU)
	$(MAKE) -C extest CPU=$(CPU) clean

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(MAKE) -C extest CPU=$(CPU) cleanall
