CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all: libipoctal.$(CPU).a


CFLAGS=-g -Wall

libipoctal.$(CPU).a: libipoctal.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libipoctal.$(CPU).{o,a}

cleanall:
	$(RM) libipoctal.{$(ALL_CPUS_COMMAS)}.{o,a}
