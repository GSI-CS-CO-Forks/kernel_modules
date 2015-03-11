CPU=L865
CFLAGS=-g -Wall
HEADERS=../lib -I../driver
LDFLAGS += -L/acc/local/$(CPU)/drv/msatt/1.0/lib
TARGET=msattntst

include /acc/src/dsc/co/Make.auto

all: $(TARGET).$(CPU)

$(TARGET).$(CPU):
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -D$(CPU) -I$(HEADERS) $(TARGET).c -o ./obj/$(TARGET).o
	$(LINK.c) -o./obj/$@ ./obj/$(TARGET).o $(LDFLAGS) $^ -lmsatt -lm -L../lib/

clean:
	rm -rf ./obj/ 
