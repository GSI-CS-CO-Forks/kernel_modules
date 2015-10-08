CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

ifndef ACCS
    ACCS=lab oper oplhc
endif

all: driver

driver:
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

install: install_driver

install_driver: driver pickmuxinstall
	for a in $(ACCS); do \
	    dsc_install $^ /acc/dsc/$$a/$(CPU)/$(KERNELVERSION)/pickeringmux; \
	done

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
