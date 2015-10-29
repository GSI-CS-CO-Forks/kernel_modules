CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

COMPILE_TIME:=$(shell date +%s)

DRIVER:=../driver
ICV196LB:=../lib

CFLAGS = -g -Wall -I$(ICV196LB) -I$(DRIVER) -DCOMPILE_TIME=$(COMPILE_TIME)

LDLIBS = $(ICV196LB)/libicv196.$(CPU).a

ALL  = icv196test.$(CPU)
SRCS = icv196test.c icv196Cmds.c DoCmd.c GetAtoms.c Cmds.c
HDRS = Cmds.h $(ICV196LB)/libicv196.h $(DRIVER)/icv196.h

all: $(ALL)

clean:
	$(RM) *.$(CPU)
	$(RM) *.$(CPU).o

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o

# Run on Workstation only

icv196test.$(CPU).o: $(SRCS) $(HDRS)

icv196test.$(CPU): icv196test.$(CPU).o
