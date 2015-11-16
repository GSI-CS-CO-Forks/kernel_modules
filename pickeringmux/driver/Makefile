CPU ?= L866
# include the build environment
include ../../common.mk

ifndef ACCS
    ACCS=lab oper oplhc
endif

all: driver

driver:
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(TOOLS) modules
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
