# include the build environment
include ../../common.mk

CFLAGS= -g -Wall -fPIC
CFLAGS+= -I../driver
CFLAGS+=-DCOMPILE_TIME=$(shell date +%s)

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

install:
	dsc_install libicv196.$(CPU).a /acc/local/$(CPU)/drv/icv196vme/
