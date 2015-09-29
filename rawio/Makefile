CPU ?= L866
include ../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS+=-I../vmebridge/include
all: vmeio.$(CPU)

clean:
	$(RM) vmeio.$(CPU)

cleanall:
	$(RM) vmeio.{$(ALL_CPUS_COMMAS)}
