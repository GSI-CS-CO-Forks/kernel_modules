# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: driver
driver: modules

modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)

DRIVERS_DEPLOY=\
	mod-pci.ko \
	modulbus_register.ko \
	vmod12a2.ko \
	vmod12a4.ko \
	vmod12e16.ko \
	vmod16a2.ko \
	vmoddor.ko \
	vmodio.ko \
	vmodttl.ko \

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global
