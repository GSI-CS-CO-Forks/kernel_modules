# include the build environment
include ../common.mk

CFLAGS+=-I../vmebridge/include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: vmeio.$(CPU)

clean:
	$(RM) vmeio.$(CPU)

cleanall:
	$(RM) vmeio.{$(ALL_CPUS_COMMAS)}
