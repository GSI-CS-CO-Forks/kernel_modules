CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

all: modules

modules:
	$(CP) Module.symvers.ipack Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(TOOLS) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
