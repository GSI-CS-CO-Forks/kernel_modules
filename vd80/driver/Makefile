CPU ?= L866

ifeq ($(CPU),L865)
KVER=3.2.43-rt63
endif

ifeq ($(CPU),L866)
KVER=3.6.11-rt30.25.el6rt.x86_64
endif

KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)
GIT_VERSION = $(shell git describe --dirty --always --tags --long)

CFLAGS=-g -Wall

all: modules

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) modules
	rm -rf ./$(CPU)
	mkdir ./$(CPU)
	mkdir ./$(CPU)/$(KVER)
	mv vd80.ko ./$(CPU)/$(KVER)/vd80.ko
	chmod 664 ./$(CPU)/$(KVER)/vd80.ko
	rm -f *.o

clean:
	rm -f vd80.ko
	rm -f ./$(CPU)/$(KVER)/vd80.ko
	rm -f *.o

install: ./$(CPU)/$(KVER)/vd80.ko
	@echo "please specify one of {install-lab|install-oper|install-lhc|install-all}"

install-all: install-lab install-oper install-oplhc

install-lab: ./$(CPU)/$(KVER)/vd80.ko
	dsc_install ./$(CPU)/$(KVER)/vd80.ko /acc/dsc/lab/$(CPU)/$(KVER)/vd80;

install-oper: ./$(CPU)/$(KVER)/vd80.ko
	dsc_install ./$(CPU)/$(KVER)/vd80.ko /acc/dsc/oper/$(CPU)/$(KVER)/vd80;

install-oplhc: ./$(CPU)/$(KVER)/vd80.ko
	dsc_install ./$(CPU)/$(KVER)/vd80.ko /acc/dsc/oplhc/$(CPU)/$(KVER)/vd80;
