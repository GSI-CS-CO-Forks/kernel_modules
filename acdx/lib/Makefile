#================================================================
# Makefile to produce acdxing library
#================================================================

# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS= -g -Wall
CFLAGS+= -I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LIBS=libacdx.$(CPU).a libamp.$(CPU).a

all:$(LIBS)

libacdx.$(CPU).a: libacdx.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

libamp.$(CPU).a: libamp.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) libacdx.$(CPU).{o,a}
	$(RM) libamp.$(CPU).{o,a}

cleanall:
	$(RM) libacdx.{$(ALL_CPUS_COMMAS)}.{o,a}
	$(RM) libamp.{$(ALL_CPUS_COMMAS)}.{o,a}

LIBS_LIST=$(LIBS)

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=libamp.h libampP.h libacdx.h

# use default instalation rule for headers
install: install_headers_global
