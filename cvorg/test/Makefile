CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS += -g -Wall -D_GNU_SOURCE

HEADERS := -I../include/ \
	-I../../include/ \
	-I../extest/ \
	-I../lib \
	-I../ad9516

LOADLIBES := -L../lib/ -L../ad9516
LDLIBS=-lcvorg.$(CPU) -lreadline -lad9516.$(CPU) -lrt -lm
all: extest.o cvorgtest.$(CPU) 

cvorgtest.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) $(HEADERS) cvorgtest.c -o ./obj/cvorgtest.o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/cvorgtest.o ./obj/extest.o ./obj/cmd_noskel.o $(LOADLIBES) $(LDLIBS)

extest.o:
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) $(HEADERS) ../extest/cmd_noskel.c -o ./obj/cmd_noskel.o
	$(CC) $(CFLAGS) -c -D$(CPU) $(HEADERS) ../extest/extest.c -o ./obj/extest.o

clean:
	rm -rf ./obj/ *.o
