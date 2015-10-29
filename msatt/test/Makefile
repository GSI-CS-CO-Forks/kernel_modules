CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS=-g -Wall
CFLAGS+=-I../lib -I../driver
LDFLAGS += -L../lib
LDLIBS += -lmsatt.$(CPU)

all: msattntst.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
