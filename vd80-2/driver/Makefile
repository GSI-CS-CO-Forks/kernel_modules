CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

all: modules

modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(TOOLS) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)

install: ./$(CPU)/$(KVER)/vd80-2.ko
	@echo "please specify one of {install-lab|install-oper|install-lhc|install-all}"

install-all: install-lab install-oper install-oplhc

install-lab: ./$(CPU)/$(KVER)/vd80-2.ko
	dsc_install ./$(CPU)/$(KVER)/vd80-2.ko /acc/dsc/lab/$(CPU)/$(KVER)/vd80;

install-oper: ./$(CPU)/$(KVER)/vd80-2.ko
	dsc_install ./$(CPU)/$(KVER)/vd80-2.ko /acc/dsc/oper/$(CPU)/$(KVER)/vd80;

install-oplhc: ./$(CPU)/$(KVER)/vd80-2.ko
	dsc_install ./$(CPU)/$(KVER)/vd80-2.ko /acc/dsc/oplhc/$(CPU)/$(KVER)/vd80;
