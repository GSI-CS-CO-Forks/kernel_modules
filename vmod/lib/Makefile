# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: lib

LIBS=libvmod.$(CPU).a
OBJS=\
	libvmod12a2.$(CPU).o \
	libvmod12a4.$(CPU).o \
	libvmod16a2.$(CPU).o \
	libvmod12e16.$(CPU).o \
	libvmod.$(CPU).o \
	libvmodttl.$(CPU).o \
	libvmoddor.$(CPU).o \

CFLAGS=-g -Wall -fPIC
CFLAGS += -I ../include -I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

lib: $(LIBS)
shlib: $(LIBS:.a=.so)

libvmod.$(CPU).so: $(OBJS)
	$(CC) -shared -o $@ $^
libvmod.$(CPU).a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) *.$(CPU).{o,so,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) *.{o,so,a}

LIBS_LIST=$(LIBS)

# use default instalation rule for libs
install: install_libs_global
