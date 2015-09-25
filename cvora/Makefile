CPU?=L866
include ../common.mk
include /acc/dsc/src/co/Make.auto

all: modules libs

#used for mv
DRIVER_OBJ=cvora.o cvora.mod.o

modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules;
	$(MKDIR) -p $(CPU)/$(KVER)
	mv $(DRIVER_OBJ) $(CPU)/$(KVER)
	mv *.ko $(CPU)/$(KVER)
	ln -s $(CPU)/$(KVER)/cvora.ko

clean:
	$(RM) *.$(CPU).{o,so,a}
	$(RM) -r $(CPU)
	$(RM) cvora.ko;
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean;
#	$(MAKE) -C test CPU=$(CPU) clean

cleanall: clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) -r {$(ALL_CPUS_COMMAS)}
#	$(MAKE) -C test CPU=$(CPU) cleanall
	$(MAKE) -C doc clean

docs:
	$(MAKE) -C doc

CFLAGS= -g -Wall -fPIC
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"


libs: libcvora.$(CPU).so libcvora.$(CPU).a

libcvora.$(CPU).o: libcvora.c libcvora.h
libcvora.$(CPU).so: libcvora.$(CPU).o
	$(CC) $(CFLAGS) -shared -o $@ $^
libcvora.$(CPU).a: libcvora.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

DRVRPATH = /acc/dsc/$(ACC)/$(CPU)/$(KVER)/cvora
LIBPATH = /acc/local/$(CPU)/drv/cvora

install-lab:
	$(MAKE) install-driver ACC=lab
install-oper:
	$(MAKE) install-driver ACC=oper
install-all: install-lab install-oper install-oplhc install-opctf

install-driver:
	dsc_install $(CPU)/$(KVER)/cvora.ko $(DRVRPATH)

install-lib:
	dsc_install libcvora.h libcvora.$(CPU).a $(LIBPATH)
install-test:
	dsc_install test/cvoratest.py $(LIBPATH)
	dsc_install libcvora.$(CPU).so $(LIBPATH)/cvora
