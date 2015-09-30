CPU?=L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

OBJECTDIR=object
CFLAGS += -I.  -I../../include -I/acc/sys/$(CPU)/usr/include/libxml2/
# needed for L867 at least now, hopefully cross compiling will be fixed one day
LDFLAGS+=-L/acc/sys/$(CPU)/usr/lib64/
LDLIBS=-lxml2 -lz
ifeq ($(CPU),ppc4)
	LDLIBS += -liberty
	LDFLAGS += -L /acc/local/ppc4/lib
	LDFLAGS += -L /acc/sys/623/cdk/linux-elf-ppc/lib/
	CPU_OBJECTS = extra_for_lynx.$(CPU).o
else
	CPU_OBJECTS = inst-linux.$(CPU).o
endif

all: lib inst

lib: libutils.$(CPU).a
inst: instprog.$(CPU)

instprog.$(CPU): instprog.$(CPU).o libutils.$(CPU).a
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

libutils.$(CPU).a: $(CPU_OBJECTS) libinst.$(CPU).o inst-utils.$(CPU).o
	$(AR) r $@ $^

clean:
	$(RM) -r $(OBJECTDIR)
	$(RM) *.$(CPU).{o,a}
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}
	$(RM) *.{$(ALL_CPUS_COMMAS)}
