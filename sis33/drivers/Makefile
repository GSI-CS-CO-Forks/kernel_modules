# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

.PHONY: all clean cleanall

all: modules

modules:
	cp Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MKDIR) -p ./$(CPU)/$(KVER)
	mv *.o *.ko ./$(CPU)/$(KVER)

clean:
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean
	$(RM) -r $(CPU)

cleanall: clean
	$(RM) -r $(ALL_CPUS)

DRIVERS_DEPLOY=sis33.ko sis3300.ko sis3320.ko

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global
