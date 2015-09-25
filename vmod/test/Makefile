CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

ifeq ($(CPU), ppc4)
all: tstlibttl.$(CPU) tstlibdor.$(CPU) tstlib12a2.$(CPU) tstlib16a2.$(CPU) tstlib12e16.$(CPU) 
CFLAGS= \
        -I../include/ \
        -I/acc/src/dsc/drivers/dioaiofclty/include \
        -I/acc/dsc/lab/ppc4/include \
        -D$(CPU)

LDFLAGS+=-L/ps/local/ppc4/lib/ -L../lib/

LDLIBS=-lvmodttl2dioaio -lvmoddor2dioaio -lvmod2dioaio -ldrvrutil -lerr -luio -lvme -lbus 

else

CFLAGS += -I ../include/ -I./ -I../driver/
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
endif 

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
	$(RM) *.ppc4
