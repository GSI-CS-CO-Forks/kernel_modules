# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

lib: libmsatt.$(CPU).a
OBJS=libmsatt.$(CPU).o

CFLAGS=-g -Wall -fPIC
CFLAGS+=-I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: lib

libmsatt.$(CPU).so: $(OBJS)
	$(CC) -shared -o $@ $^

libmsatt.$(CPU).a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) libmsatt.$(CPU).{o,so,a}

cleanall:
	$(RM) libmsatt.{$(ALL_CPUS_COMMAS)}.{o,so,a}

LIBS_LIST=libmsatt.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=libmsatt.h

# use default instalation rule for headers
install: install_headers_global
