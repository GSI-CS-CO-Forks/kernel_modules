#
#       Makefile for building pciioconfig lib
#
#

CPU=L865

# Include standard autorules
include /acc/src/dsc/co/Make.auto

# Flags to compile test program and lib
CFLAGS=-Wall -g -I.

# Test program src
SRCS=test_pciioconfig.c

# Lib src
LIBSRCS=crateconfig.c

CONFIGS= crateconfig.21.pp-300 crateconfig.21.pp-cp2 crateconfig.3.S845WD1 crateconfig.5.pp-300 crateconfig.5.pp-cp2

ALL=libpciioconfig.$(CPU).a $(SRCS:.c=.$(CPU))

all: $(ALL)

$(SRCS:.c=.$(CPU)): $(SRCS) libpciioconfig.$(CPU).a

libpciioconfig.$(CPU).a: $(LIBSRCS:.c=.$(CPU).o)
	$(RM) $@
	$(AR) -crv $@ $^

clean:
	rm -f $(ALL) ,* *.$(CPU).o \#* *~

install:    libpciioconfig.$(CPU).a crateconfig.h $(CONFIGS)
	dsc_install $^ /acc/local/$(CPU)/pciioconfig
