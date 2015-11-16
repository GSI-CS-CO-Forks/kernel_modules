#================================================================
# Makefile to produce acdxing library
#================================================================

# include the build environment
include ../../common.mk

CFLAGS= -g -Wall
CFLAGS+= -I../driver

INSTFILES=libacdx.$(CPU).a libacdx.h ../driver/acdxdrvr.h ../driver/acdxdrvrP.h libamp.$(CPU).a libamp.h

ACCS=

all:$(INSTFILES)

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

install: $(INSTFILES)
	cp libacdx.$(CPU).a /acc/local/$(CPU)/acdx/libacdx.a
	cp libamp.$(CPU).a /acc/local/$(CPU)/acdx/libamp.a
	rm -f /acc/local/$(CPU)/acdx/acdxdrvr.h
	rm -f /acc/local/$(CPU)/acdx/acdxdrvrP.h
	rm -f /acc/local/$(CPU)/acdx/libacdx.h
	rm -f /acc/local/$(CPU)/acdx/libamp.h
	rm -f /acc/local/$(CPU)/acdx/libampP.h
	cp ../driver/acdxdrvr.h /acc/local/$(CPU)/acdx
	cp ../driver/acdxdrvrP.h /acc/local/$(CPU)/acdx
	cp libamp.h /acc/local/$(CPU)/acdx
	cp libampP.h /acc/local/$(CPU)/acdx
	cp libacdx.h /acc/local/$(CPU)/acdx
