# generated automatically by encore, date 2012-02-15 10:29:49.901731

CPU=L865
KVER=2.6.24.7-rt27
KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
include /acc/dsc/src/co/Make.auto

DRIVER = ctc
DRIVER_NAME = "$(DRIVER)"
LIBNAME = lib$(DRIVER)

COMPILE_TIME = $(shell date +%s)
CFLAGS= -g -Wall -I. -I ../../driver
CFLAGS += -I /acc/local/$(CPU)/include/
CFLAGS += -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

GIT_VERSION = $(shell git describe --tags --dirty --long --always)

MODLIBS = $(LIBNAME).$(CPU).a 
MODLIBOBJS = $(LIBNAME).$(CPU).o $(DRIVER)_regs.$(CPU).o
ENCORELIBS = libencore.$(CPU).a

SRCS=$(LIBNAME).c $(LIBNAME).h

all: modules libs
libs: $(MODLIBS) $(ENCORELIBS)
modules:
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) 'DRIVER_NAME=$(DRIVER_NAME)' modules
clean:
	rm -f *.o *.a *.so
	make -C $(KERNELSRC) M=`pwd` clean

$(LIBNAME).$(CPU).o: $(SRCS)

$(LIBNAME).$(CPU).a: $(MODLIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

libencore.$(CPU).o: libencore.c libencore.h
libencore.$(CPU).a: libencore.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^

solibs: libencore.$(CPU).so $(LIBNAME).$(CPU).so
$(LIBNAME).$(CPU).so: $(MODLIBOBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)
libencore.$(CPU).so: libencore.$(CPU).o
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

ACC = lab
install-test: solibs
	mkdir -p /acc/local/$(CPU)/drv/ctc/tools
	dsc_install libencore.$(CPU).so $(LIBNAME).$(CPU).so \
		/acc/local/$(CPU)/drv/ctc/tools
	install -b -m 755 ctctest
	ln -sf /acc/local/$(CPU)/drv/ctc/tools/ctctest /acc/dsc/$(ACC)/$(CPU)/bin
