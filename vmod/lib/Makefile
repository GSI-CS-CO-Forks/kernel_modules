CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all: lib

ifeq ($(CPU), ppc4)

LIBS=libvmod2dioaio.a libvmodttl2dioaio.a libvmoddor2dioaio.a
lib: $(LIBS)
OBJS=libvmod12a22dioaio.o libvmod16a22dioaio.o libvmod12e162dioaio.o

HEADERS=../include \
        -I/acc/src/dsc/drivers/dioaiofclty/include \
        -I/acc/src/dsc/drivers/ioconfig/include \
        -I/acc/src/dsc/drivers/rt/include/ \
        -I/acc/dsc/lab/ppc4/include

CFLAGS=-g -Wall

libvmod12a22dioaio.o:
	$(CC) $(CFLAGS) -c -I$(HEADERS) libvmod12a22dioaio.c -o $@
libvmod16a22dioaio.o:
	$(CC) $(CFLAGS) -c -I$(HEADERS) libvmod16a22dioaio.c -o $@
libvmod12e162dioaio.o:
	$(CC) $(CFLAGS) -c -I$(HEADERS) libvmod12e162dioaio.c -o $@

libvmodttl2dioaio.o:
	$(CC) $(CFLAGS) -c -I$(HEADERS) libvmodttl2dioaio.c -o $@
libvmoddor2dioaio.o:
	$(CC) $(CFLAGS) -c -I$(HEADERS) libvmoddor2dioaio.c -o $@

libvmod2dioaio.so: $(OBJS)
	gcc -shared -o $@ $^
libvmod2dioaio.a: $(OBJS)
	$(AR) r $@ $^

libvmod12a22dioaio.a: libvmod12a22dioaio.o
	$(AR) r $@ $^
libvmod16a22dioaio.a: libvmod16a22dioaio.o
	$(AR) r $@ $^
libvmod12e162dioaio.a: libvmod12e162dioaio.o
	$(AR) r $@ $^

libvmodttl2dioaio.a: libvmodttl2dioaio.o
	$(AR) r $@ $^
libvmodttl2dioaio.so: libvmodttl2dioaio.o
	$(CC) -shared -o $@ $^
libvmoddor2dioaio.a: libvmoddor2dioaio.o
	$(AR) r $@ $^
libvmoddor2dioaio.so: libvmoddor2dioaio.o
	$(CC) -shared -o $@ $^

clean:
	$(RM) *.{o,so,a}

else

LIBS=libvmod.$(CPU).a libvmodttl.$(CPU).a libvmoddor.$(CPU).a
OBJS=libvmod12a2.$(CPU).o libvmod12a4.$(CPU).o libvmod16a2.$(CPU).o libvmod12e16.$(CPU).o

CFLAGS=-g -Wall -fPIC
CFLAGS += -I ../include -I../driver

lib: $(LIBS)
shlib: $(LIBS:.a=.so)

libvmod.$(CPU).so: $(OBJS)
	$(CC) -shared -o $@ $^
libvmod.$(CPU).a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

libvmodttl.$(CPU).a: libvmodttl.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
libvmodttl.$(CPU).so: libvmodttl.$(CPU).o
	$(CC) -shared -o $@ $^

libvmoddor.$(CPU).a: libvmoddor.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
libvmoddor.$(CPU).so: libvmoddor.$(CPU).o
	$(CC) -shared -o $@ $^

clean:
	$(RM) *.$(CPU).{o,so,a}

endif


cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) *.{o,so,a}
