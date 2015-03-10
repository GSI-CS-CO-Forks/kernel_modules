CPU=L865
CFLAGS=-g -Wall
HEADERS=../lib -I../driver
LDFLAGS += -L/acc/local/$(CPU)/drv/msatt/1.0/lib

include /acc/src/dsc/co/Make.auto

all: tstmsatt.$(CPU)

tstmsatt.$(CPU):
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) tstmagatt.c -o ./obj/tstmagatt.o
	$(LINK.c) -o./obj/$@ ./obj/tstmagatt.o $(LDFLAGS) $^ -lmsatt -lm -L../lib/

clean:
	rm -rf ./obj/ 
