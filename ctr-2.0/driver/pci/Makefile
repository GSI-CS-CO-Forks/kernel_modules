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
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) modules
	cp ctrp.ko ctrp.$(KVER).ko
	rm -f ctrp.ko
	ln -s ctrp.$(KVER).ko ctrp.ko
	mkdir -p ./$(CPU)/$(KVER)
	cp ctrp.$(KVER).ko  ./$(CPU)/$(KVER)/ctrp.ko

install: ./$(CPU)/$(KVER)/ctrp.ko
	@echo "please specify one of {install-lab|install-oper|install-lhc|install-all}"

install-all: install-lab install-oper install-oplhc

install-lab: ./$(CPU)/$(KVER)/ctrp.ko
	mkdir -p /acc/dsc/lab/$(CPU)/$(KVER)/ctrp
	dsc_install ./$(CPU)/$(KVER)/ctrp.ko /acc/dsc/lab/$(CPU)/$(KVER)/ctrp;

install-oper: ./$(CPU)/$(KVER)/ctrp.ko
	mkdir -p /acc/dsc/oper/$(CPU)/$(KVER)/ctrp
	dsc_install ./$(CPU)/$(KVER)/ctrp.ko /acc/dsc/oper/$(CPU)/$(KVER)/ctrp;

install-oplhc: ./$(CPU)/$(KVER)/ctrp.ko
	mkdir -p /acc/dsc/oplhc/$(CPU)/$(KVER)/ctrp
	dsc_install ./$(CPU)/$(KVER)/ctrp.ko /acc/dsc/oplhc/$(CPU)/$(KVER)/ctrp;
