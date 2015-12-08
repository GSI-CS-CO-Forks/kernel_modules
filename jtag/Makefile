# include the build environment
include ../common.mk

CFLAGS = -g -I../vmebridge/include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
# use libvmebus from the repo
LDLIBS = ../vmebridge/lib/libvmebus.$(CPU).a
ALL  = jtag.$(CPU)
SRCS = jtag.c lenval.c micro.c ports.c smemio.c
HDRS = lenval.h micro.h ports.h

.PHONY: all clean cleanall

all: $(ALL)

jtag.$(CPU): $(SRCS:.c=.$(CPU).o)

clean:
	$(RM) *.$(CPU)
	$(RM) *.$(CPU).o

cleanall: 
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
