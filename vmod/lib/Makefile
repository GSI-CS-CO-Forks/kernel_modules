CPU=L865
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

else

LIBS=libvmod.$(CPU).a libvmodttl.$(CPU).a libvmoddor.$(CPU).a
OBJS=libvmod12a2.$(CPU).o libvmod12a4.$(CPU).o libvmod16a2.$(CPU).o libvmod12e16.$(CPU).o

CFLAGS=-g -Wall
CFLAGS += -I ../include -I../driver

lib: $(LIBS)

libvmod12a2.$(CPU).o:	libvmod12a2.c
	$(CC) $(CFLAGS) -c -o $@ $^
libvmod12a4.$(CPU).o:   libvmod12a4.c
	$(CC) $(CFLAGS) -c -o $@ $^
libvmod16a2.$(CPU).o:   libvmod16a2.c
	$(CC) $(CFLAGS) -c -o $@ $^
libvmod12e16.$(CPU).o:  libvmod12e16.c
	$(CC) $(CFLAGS) -c -o $@ $^
libvmodttl.$(CPU).o:  libvmodttl.c
	$(CC) $(CFLAGS) -c -o $@ $^
libvmoddor.$(CPU).o:  libvmoddor.c
	$(CC) $(CFLAGS) -c -o $@ $^

libvmod.$(CPU).so: $(OBJS)
	gcc -shared -o $@ $^
libvmod.$(CPU).a: $(OBJS)
	ar r $@ $^

libvmodttl.$(CPU).a: libvmodttl.$(CPU).o
	ar r $@ $^
libvmodttl.$(CPU).so: libvmodttl.$(CPU).o
	gcc -shared -o $@ $^
libvmoddor.$(CPU).a: libvmoddor.$(CPU).o
	ar r $@ $^
libvmoddor.$(CPU).so: libvmoddor.$(CPU).o
	gcc -shared -o $@ $^

endif

clean:
	rm -f *.o *.a *.pyc *.so
