CPU=L865
CFLAGS=-g -Wall
HEADERS=../lib -I../driver

all: tstmsatt.$(CPU)

tstmsatt.$(CPU):
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) tstmagatt.c -o ./obj/tstmagatt.o
	$(LINK.c) -o./obj/$@ ./obj/tstmagatt.o $(LDFLAGS) $^ -lmsatt -lm -L../lib/

clean:
	rm -rf ./obj/ 
