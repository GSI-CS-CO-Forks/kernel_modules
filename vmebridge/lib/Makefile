# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS+= -I../include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: libvmebus.$(CPU).a

libvmebus.$(CPU).a: libvmebus.$(CPU).o
	$(AR) rv $@ $^

libvmebus.$(CPU).o: libvmebus.c libvmebus.h

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}

LIBS_LIST=libvmebus.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=libvmebus.h

# use default instalation rule for headers
install: install_headers_global
