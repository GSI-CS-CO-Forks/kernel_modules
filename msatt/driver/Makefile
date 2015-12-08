# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)

DRIVERS_DEPLOY=msatt.ko

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global
