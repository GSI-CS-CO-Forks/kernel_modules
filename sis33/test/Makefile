CPU?=L866
# include the build environment
include ../../common.mk

VPATH = ../../utils/extest ../lib
INCDIR := ../include
CFLAGS := -Wall -g -I$(INCDIR) -I../../include -I../../utils/extest -I../lib \
	-I../../utils/user -DDRIVER_NAME=\"sis33\"
LIBUTILSPATH := ../../utils/user
LIBUTILS := $(LIBUTILSPATH)/libutils.$(CPU).a
LDLIBS := -lxml2 -lreadline -lrt
LDFLAGS := $(LIBUTILS)

#L865 requires tempcap for readline
ifeq ($(CPU),L865)
LDFLAGS+=-ltermcap
endif

HSIS33 := $(INCDIR)/sis33.h

all: sis33test.$(CPU)

.PHONY: all clean cleanall

sis33test.$(CPU): sis33test.$(CPU).o sis33acq.$(CPU).o sis33dev.$(CPU).o \
	extest.$(CPU).o cmd_generic.$(CPU).o $(LIBUTILS)
sis33test.$(CPU).o: sis33test.c $(HSIS33) extest.h
sis33acq.$(CPU).o: sis33acq.c sis33acq.h $(HSIS33)
sis33dev.$(CPU).o: sis33dev.c sis33dev.h $(HSIS33)
extest.$(CPU).o: extest.c extest.h extest_common.h
cmd_generic.$(CPU).o: cmd_generic.c extest.h extest_common.h cmd_handlers.h

$(LIBUTILS):
	$(MAKE) -C $(LIBUTILSPATH) CPU=$(CPU)

clean:
	$(RM) *.$(CPU)
	$(RM) *.$(CPU).{o,a}
	$(MAKE) -C $(LIBUTILSPATH) CPU=$(CPU) clean

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(MAKE) -C $(LIBUTILSPATH) CPU=$(CPU) cleanall
