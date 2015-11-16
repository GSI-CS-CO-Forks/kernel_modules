CPU?=L866
# include the build environment
include ../../common.mk

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
