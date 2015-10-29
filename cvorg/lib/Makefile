CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

CFLAGS := -Wall -g -D_GNU_SOURCE -I../include -I../../include -I../ad9516
LIBCVORG := libcvorg.$(CPU).a 
HLIBCVORG := ../include/libcvorg.h
HCVORG := ../include/cvorg.h
OBJAD9516 := ad9516.$(CPU).o
EXAMPLES_DIR := examples
DOXY_DIR := doxygen_output
DOXY_FILES := cvorg.c error.c $(HLIBCVORG) $(HCVORG)
DOXY_INSTDIR := /acc/doc/html/private/coht/doxy/cvorg

all: $(LIBCVORG) examples

.PHONY: all clean cleanall examples

cvorg.$(CPU).o: cvorg.c $(HCVORG) $(HLIBCVORG) libinternal.h cvorgdev.h

libcvorg.$(CPU).a: cvorg.$(CPU).o cvorgdev.$(CPU).o error.$(CPU).o $(OBJAD9516)
	$(AR) rv $(LIBCVORG) $^

cvorgdev.$(CPU).o: cvorgdev.c cvorgdev.h

error.$(CPU).o: error.c libinternal.h

libcvorg.$(CPU).so: cvorg.$(CPU).o cvorgdev.$(CPU).o error.$(CPU).o $(OBJAD9516)
	$(CC) -shared -o $@ $^

examples: $(LIBCVORG)
	$(MAKE) -C $(EXAMPLES_DIR) CPU=$(CPU)

clean:
	$(RM) *.$(CPU).{o,so,a}
	$(MAKE) clean -C $(EXAMPLES_DIR)

cleanall: doxy_clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(MAKE) cleanall -C $(EXAMPLES_DIR)

doxy: doxy_clean $(DOXY_FILES)
	../../doxy/doxy.sh -n"libcvorg" -o"$(DOXY_DIR)" $(DOXY_FILES)

doxy_clean:
	$(RM) -r $(DOXY_DIR)

doxy_install: doxy
	$(MKDIR) -p $(DOXY_INSTDIR)
	$(CP) -r $(DOXY_DIR)/html/* $(DOXY_INSTDIR)/
