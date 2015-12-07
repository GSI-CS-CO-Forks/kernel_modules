# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS= -g -Wall -fPIC
CFLAGS+= -I../driver
CFLAGS+=-DCOMPILE_TIME=$(shell date +%s)
CFLAGS+=-DGIT_VERSION=\"$(GIT_VERSION)\"

SRCS = libicv196.c libicv196.h

all: libicv196.$(CPU).so libicv196.$(CPU).a

libicv196.$(CPU).o: $(SRCS)

libicv196.$(CPU).so: libicv196.$(CPU).o

libicv196.$(CPU).a: libicv196.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) libicv196.$(CPU).{o,so,a}

cleanall: docs_clean
	$(RM) libicv196.{$(ALL_CPUS_COMMAS)}.{o,so,a}

docs:
	doxygen -s -g t
	sed -f Doxyfile.sed t > Doxyfile
	$(RM) t
	doxygen

docs-update: docs
	rsync -av html/ /acc/doc/html/private/coht/doxy/icv196

docs_clean:
	$(RM) -r html
	$(RM) t

LIBS_LIST=libicv196.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=libicv196.h

# use default instalation rule for headers
install: install_headers_global
