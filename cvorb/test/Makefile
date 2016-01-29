# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

VPATH+=../../utils/extest ../../utils/install

CFLAGS = -Wall -g \
	-I../include \
	-I../lib  \
	-I../../utils/extest \
	-I../../include \
	-I/acc/sys/$(CPU)/usr/include/libxml2/ \
	-DDRIVER_NAME=\"cvorb\" \

CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LOADLIBES := -L../lib
LDLIBS= -lcvorb.$(CPU) -lreadline -lz -lrt -lm -lxml2

#L865 requires tempcap for readline
ifeq ($(CPU),L865)
LDLIBS+=-ltermcap
endif


all: cvorbtest.$(CPU)

.PHONY: all clean
cvorbtest.$(CPU): cvorbtest.$(CPU).o time_stamp_counter.$(CPU).o $(LIBUTILS) extest.$(CPU).o libinst.$(CPU).o cmd_generic.$(CPU).o

cvorbtest.$(CPU).o: cvorbtest.c time_stamp_counter.c time_stamp_counter.h

clean:
	$(RM) *.$(CPU).o
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) *.{$(ALL_CPUS_COMMAS)}

install:
