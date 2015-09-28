CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

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

cleanall:
	$(RM) libicv196.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) -r html

docs:
	doxygen -s -g t
	sed -f Doxyfile.sed t > Doxyfile
	rm t
	doxygen

docs-update: docs
	rsync -av html/ /acc/doc/html/private/coht/doxy/icv196


install:
	dsc_install libicv196.$(CPU).a /acc/local/$(CPU)/drv/icv196vme/
