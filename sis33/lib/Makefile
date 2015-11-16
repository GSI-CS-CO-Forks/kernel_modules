# include the build environment
include ../../common.mk

INCDIR := ../include
CFLAGS := -Wall -fPIC -g -I$(INCDIR) -I../../include
LIBSIS33 := libsis33.$(CPU).a 
HLIBSIS33 := $(INCDIR)/libsis33.h
HSIS33 := $(INCDIR)/sis33.h
EXAMPLES_DIR := examples
DOXY_DIR := doxygen_output
DOXY_FILES := sis33.c error.c $(HLIBSIS33) $(HSIS33)
DOXY_INSTDIR := /acc/doc/html/private/coht/doxy/sis33

all: $(LIBSIS33) examples libsis33.$(CPU).so

.PHONY: all clean cleanall examples

sis33.$(CPU).o: sis33.c $(HSIS33) $(HLIBSIS33) libinternal.h sis33acq.h sis33dev.h

libsis33.$(CPU).a: sis33.$(CPU).o sis33acq.$(CPU).o sis33dev.$(CPU).o error.$(CPU).o
	$(AR) rv $(LIBSIS33) $^
	$(RANLIB) $(LIBSIS33)

sis33acq.$(CPU).o: sis33acq.c sis33acq.h
sis33dev.$(CPU).o: sis33dev.c sis33dev.h
error.$(CPU).o: error.c libinternal.h

libsis33.$(CPU).so: sis33.$(CPU).o sis33acq.$(CPU).o sis33dev.$(CPU).o error.$(CPU).o
	$(CC) -shared -o $@ $^

examples: $(LIBSIS33)
	$(MAKE) -C $(EXAMPLES_DIR) CPU=$(CPU)

clean:
	$(MAKE) clean -C $(EXAMPLES_DIR) CPU=$(CPU)
	$(RM) *.$(CPU).{o,so,a}

cleanall: doxy_clean
	$(MAKE) cleanall -C $(EXAMPLES_DIR)
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}


doxy: doxy_clean $(DOXY_FILES)
	../../doxy/doxy.sh -n"libsis33" -o"$(DOXY_DIR)" $(DOXY_FILES) 2>&1

doxy_clean:
	$(RM) -r $(DOXY_DIR)

doxy_install: doxy
	mkdir -p $(DOXY_INSTDIR)
	cp -r $(DOXY_DIR)/html/* $(DOXY_INSTDIR)/
