# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS += -g -Wall
CFLAGS += -I../driver
CFLAGS += -I../../pciioconfig/lib
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: libpickeringmux.$(CPU).a

libpickeringmux.$(CPU).a: pickeringmuxlib.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) libpickeringmux.$(CPU).a
	$(RM) pickeringmuxlib.$(CPU).o

cleanall:
	$(RM) libpickeringmux.{$(ALL_CPUS_COMMAS)}.a
	$(RM) pickeringmuxlib.{$(ALL_CPUS_COMMAS)}.o

LIBS_LIST=libpickeringmux.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=pickeringmuxlib.h

#use default instalation rule for headers
install: install_headers_global
