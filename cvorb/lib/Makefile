# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS := -Wall -g -I../include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LIBCVORB := libcvorb.$(CPU).a
HLIBCVORB := ../include/libcvorb.h
HCVORB := ../include/cvorb.h
EXAMPLES_DIR := examples
DOXY_DIR := doxygen_output
DOXY_FILES := cvorb.c error.c $(HLIBCVORB) $(HCVORB)
DOXY_INSTDIR := /acc/doc/html/private/coht/doxy/cvorb

all: $(LIBCVORB)

.PHONY: all clean examples

cvorb.$(CPU).o: cvorb.c $(HCVORB) $(HLIBCVORB) libinternal.h cvorbdev.h 
	$(CC) $(CFLAGS) -c -I. cvorb.c -o $@

libcvorb.$(CPU).a: cvorb.$(CPU).o cvorbdev.$(CPU).o error.$(CPU).o $(OBJAD9516)
	$(AR) rv $(LIBCVORB) $^

cvorbdev.$(CPU).o: cvorbdev.c cvorbdev.h
	$(CC) $(CFLAGS) -c -I. cvorbdev.c -o $@

error.$(CPU).o: error.c libinternal.h
	$(CC) $(CFLAGS) -c -I. error.c -o $@

libcvorb.$(CPU).so: cvorb.$(CPU).o cvorbdev.$(CPU).o error.$(CPU).o $(OBJAD9516)
	$(CC) -shared -o $@ $^

examples:
	$(MAKE) -C $(EXAMPLES_DIR) CPU=$(CPU)

clean:
	$(RM) *.$(CPU).{o,a}

cleanall: doxy_clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}

doxy: doxy_clean $(DOXY_FILES)
	../../doxy/doxy.sh -n"libcvorb" -o"$(DOXY_DIR)" $(DOXY_FILES)

doxy_clean:
	$(RM) -r $(DOXY_DIR)

doxy_install: doxy
	$(MKDIR) -p $(DOXY_INSTDIR)
	$(CP) -r $(DOXY_DIR)/html/* $(DOXY_INSTDIR)/

LIBS_LIST=libcvorb.$(CPU).a

# use default instalation rule for libs
install: install_libs_global
