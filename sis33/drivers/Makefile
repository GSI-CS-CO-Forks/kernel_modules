CPU?=L866
# include the build environment
include ../../common.mk

.PHONY: all clean cleanall

all: modules

modules:
	cp Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(TOOLS) modules
	$(MKDIR) -p ./$(CPU)/$(KVER)
	mv *.o *.ko ./$(CPU)/$(KVER)

clean:
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean
	$(RM) -r $(CPU)

cleanall: clean
	$(RM) -r $(ALL_CPUS)
