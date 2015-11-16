# include the build environment
include ../../common.mk

# Flags to compile test program and lib
CFLAGS=-Wall -g

# Lib src
LIBSRCS=crateconfig.c

CONFIGS= crateconfig.21.pp-300 crateconfig.21.pp-cp2 crateconfig.3.S845WD1 crateconfig.5.pp-300 crateconfig.5.pp-cp2

all: libpciioconfig.$(CPU).a

libpciioconfig.$(CPU).a: $(LIBSRCS:.c=.$(CPU).o)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) *.$(CPU).o
	$(RM) libpciioconfig.$(CPU).{so,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) libpciioconfig.{$(ALL_CPUS_COMMAS)}.{so,a}

install:    libpciioconfig.$(CPU).a crateconfig.h $(CONFIGS)
	dsc_install $^ /acc/local/$(CPU)/pciioconfig
