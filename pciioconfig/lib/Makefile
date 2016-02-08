# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

# Flags to compile test program and lib
CFLAGS=-Wall -g
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

# Lib src
LIBSRCS=crateconfig.c

all: libpciioconfig.$(CPU).a

libpciioconfig.$(CPU).a: $(LIBSRCS:.c=.$(CPU).o)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) *.$(CPU).o
	$(RM) libpciioconfig.$(CPU).{so,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) libpciioconfig.{$(ALL_CPUS_COMMAS)}.{so,a}

HEADERS_LIST=crateconfig.h

# use default instalation rule for headers
install: install_headers_global

LIBS_LIST=libpciioconfig.$(CPU).a

# use default instalation rule for libs
install: install_libs_global
