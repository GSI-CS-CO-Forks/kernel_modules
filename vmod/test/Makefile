
CPU ?= L865
CFLAGS=-g -Wall

include /acc/src/dsc/co/Make.auto

ifeq ($(CPU), ppc4)
all: tstlibttl.$(CPU) tstlibdor.$(CPU) tstlib12a2.$(CPU) tstlib16a2.$(CPU) tstlib12e16.$(CPU) 
HEADERS=./ \
        -I../include/ \
        -I/acc/src/dsc/drivers/dioaiofclty/include \
        -I/acc/dsc/lab/ppc4/include

LOADLIBES+=-L/ps/local/ppc4/lib/ -L../lib/

LDLIBS=-ldrvrutil -lerr -luio -lvme -lbus

tstlibttl.$(CPU): 
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $^ tstlibttl.c -o ./obj/tstlibttl.$(CPU).o 
	$(LINK.c) -o./obj/$@ ./obj/tstlibttl.$(CPU).o $(LDFLAGS) $^ -lvmodttl2dioaio $(LOADLIBES) $(LDLIBS)

tstlibdor.$(CPU): 
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $^ tstlibdor.c -o ./obj/tstlibdor.$(CPU).o 
	$(LINK.c) -o./obj/$@ ./obj/tstlibdor.$(CPU).o $(LDFLAGS) $^ -lvmoddor2dioaio $(LOADLIBES) $(LDLIBS)

tstlib12a2.$(CPU): 
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $^ tstlib12a2.c -o ./obj/tstlib12a2.$(CPU).o 
	$(LINK.c) -o./obj/$@ ./obj/tstlib12a2.$(CPU).o $(LDFLAGS) $^ -lvmod2dioaio $(LOADLIBES) $(LDLIBS)

tstlib16a2.$(CPU): 
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $^ tstlib16a2.c -o ./obj/tstlib16a2.$(CPU).o 
	$(LINK.c) -o./obj/$@ ./obj/tstlib16a2.$(CPU).o $(LDFLAGS) $^ -lvmod2dioaio $(LOADLIBES) $(LDLIBS)

tstlib12e16.$(CPU):
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $^ tstlib12e16.c -o ./obj/tstlib12e16.$(CPU).o 
	$(LINK.c) -o./obj/$@ ./obj/tstlib12e16.$(CPU).o $(LDFLAGS) $^ -lvmod2dioaio $(LOADLIBES) $(LDLIBS)

else

CFLAGS += -I ../include/ -I./ -I../driver/
LDFLAGS += -L ../lib
LDLIBS += -lvmodttl.$(CPU)

all: tstlibttl.$(CPU) tstlibdor.$(CPU) load_test.$(CPU) tstlibttl_quick_write.$(CPU) tstlibttl_quick_read.$(CPU) \
	tstlibttl_read_device.$(CPU) \
	tstlib12a2.$(CPU) tstlib12a4.$(CPU) tstlib16a2.$(CPU) tstlib12e16.$(CPU)

tstlibttl.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlibttl.c -o ./obj/tstlibttl.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlibttl.$(CPU).o $(LDFLAGS) $(LDLIBS)

tstlibdor.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlibdor.c -o ./obj/tstlibdor.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlibdor.$(CPU).o $(LDFLAGS) -lvmoddor.$(CPU)

tstlibttl_quick_write.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlibttl_quick_write.c -o ./obj/tstlibttl_quick_write.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlibttl_quick_write.$(CPU).o $(LDFLAGS) $(LDLIBS) -lpthread

tstlibttl_quick_read.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlibttl_quick_read.c -o ./obj/tstlibttl_quick_read.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlibttl_quick_read.$(CPU).o $(LDFLAGS) $(LDLIBS) -lpthread

tstlibttl_read_device.$(CPU):
	$(CC) $(CFLAGS) -c -D$(CPU) tstlibttl_read_device.c -o ./obj/tstlibttl_read_device.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlibttl_read_device.$(CPU).o $(LDFLAGS) $(LDLIBS) -lpthread

load_test.$(CPU):
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) load_test.c -o ./obj/load_test.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/load_test.$(CPU).o $(LDFLAGS) $(LDFLAGS) $(LDLIBS) -lpthread

tstlib12a2.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlib12a2.c -o ./obj/tstlib12a2.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlib12a2.$(CPU).o $(LDFLAGS) -lvmod.$(CPU)
tstlib12a4.$(CPU):
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlib12a4.c -o ./obj/tstlib12a4.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlib12a4.$(CPU).o $(LDFLAGS) -lvmod.$(CPU)
tstlib16a2.$(CPU): 
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlib16a2.c -o ./obj/tstlib16a2.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlib16a2.$(CPU).o $(LDFLAGS) -lvmod.$(CPU)
tstlib12e16.$(CPU):
	mkdir -p obj
	$(CC) $(CFLAGS) -c -D$(CPU) tstlib12e16.c -o ./obj/tstlib12e16.$(CPU).o
	$(CC) $(CFLAGS) -o ./obj/$@ ./obj/tstlib12e16.$(CPU).o $(LDFLAGS) -lvmod.$(CPU)
endif 

clean:
	rm -rf ./obj/
