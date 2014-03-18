CPU=L866

ifeq ($(CPU),L865)
KVER=3.2.43-rt63
endif

ifeq ($(CPU),L866)
KVER=3.6.11-rt30.25.el6rt.x86_64
endif

KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)

CFLAGS=-g -Wall -I.

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) modules
	cp ctrv.ko ctrv.$(KVER).ko
	rm -f ctrv.ko
	ln -s ctrv.$(KVER).ko ctrv.ko
	mkdir -p ./$(CPU)/$(KVER)
	cp ctrv.$(KVER).ko ./$(CPU)/$(KVER)/ctrv.ko

install: ./$(CPU)/$(KVER)/ctrv.ko
	@echo "please specify one of {install-lab|install-oper|install-lhc|install-all}"

install-all: install-lab install-oper install-oplhc

install-lab: ./$(CPU)/$(KVER)/ctrv.ko
	mkdir -p /acc/dsc/lab/$(CPU)/$(KVER)/ctrv
	dsc_install ./$(CPU)/$(KVER)/ctrv.ko /acc/dsc/lab/$(CPU)/$(KVER)/ctrv;

install-oper: ./$(CPU)/$(KVER)/ctrv.ko
	mkdir -p /acc/dsc/oper/$(CPU)/$(KVER)/ctrv
	dsc_install ./$(CPU)/$(KVER)/ctrv.ko /acc/dsc/oper/$(CPU)/$(KVER)/ctrv;

install-oplhc: ./$(CPU)/$(KVER)/ctrv.ko
	mkdir -p /acc/dsc/oplhc/$(CPU)/$(KVER)/ctrv
	dsc_install ./$(CPU)/$(KVER)/ctrv.ko /acc/dsc/oplhc/$(CPU)/$(KVER)/ctrv;
