CPU ?= L866
# include the build environment
include ../common.mk

CFLAGS+=-I../vmebridge/include
all: vmeio.$(CPU)

clean:
	$(RM) vmeio.$(CPU)

cleanall:
	$(RM) vmeio.{$(ALL_CPUS_COMMAS)}
