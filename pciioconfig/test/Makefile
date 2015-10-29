CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

# Flags to compile test program and lib
CFLAGS=-Wall -g
CFLAGS+=-I../lib
LDFLAGS+=-L../lib
LDLIBS+=-lpciioconfig.$(CPU)

all: test_pciioconfig.$(CPU)

clean:
	$(RM) test_pciioconfig.$(CPU)

cleanall:
	$(RM) test_pciioconfig.{$(ALL_CPUS_COMMAS)}
