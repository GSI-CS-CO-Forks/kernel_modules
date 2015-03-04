lib: libmsatt.a
OBJS=libmsatt.o

HEADERS=./../driver

CFLAGS=-g -Wall

all: lib

libmsatt.o:
	$(CC) $(CFLAGS) -c -I. -I$(HEADERS) libmsatt.c -o $@
libmsatt.so: $(OBJS)
	gcc -shared -o $@ $^ 
libmsatt.a: $(OBJS)
	ar r $@ $^

clean:
	rm -f *.o *.a *.pyc *.so
