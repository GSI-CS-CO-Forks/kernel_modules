# include the build environment
include ../../common.mk

CFLAGS += -I ../include/ -I./ -I../driver/
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS += -L ../lib
LDLIBS += -lvmodttl.$(CPU) -lvmoddor.$(CPU) -lpthread -lvmod.$(CPU)

all: \
	tstlibttl.$(CPU) \
	tstlibdor.$(CPU) \
	load_test.$(CPU) \
	tstlibttl_quick_write.$(CPU) \
	tstlibttl_quick_read.$(CPU) \
	tstlibttl_read_device.$(CPU) \
	tstlib12a2.$(CPU) \
	tstlib12a4.$(CPU) \
	tstlib16a2.$(CPU) \
	tstlib12e16.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
