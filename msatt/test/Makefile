CPU ?= L866
# include the build environment
include ../../common.mk

CFLAGS=-g -Wall
CFLAGS+=-I../lib -I../driver
LDFLAGS += -L../lib
LDLIBS += -lmsatt.$(CPU)

all: msattntst.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
