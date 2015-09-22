CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

all: driver
driver: modules

modules: 
	$(CP) Module.symvers.vme Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CPU=$(CPU) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
