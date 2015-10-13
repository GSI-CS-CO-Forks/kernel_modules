CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all: driver
driver: modules

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
