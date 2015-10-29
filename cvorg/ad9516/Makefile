CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

INCDIR := ../include

CFLAGS= -Wall -g -I$(INCDIR)

LIBAD9516 = libad9516.$(CPU).a

all: $(LIBAD9516)

libad9516.$(CPU).a: ad9516.$(CPU).o
	$(AR) rv $(LIBAD9516) $^

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
