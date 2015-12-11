# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: driver
driver: modules

# compile with V=1 to see compilation details
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

DRIVERS_DEPLOY=cvorb.ko

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global

SCRIPTS_LIST=\
	cvorb.awk \
	install_cvorb.sh \

# use default instalation rule for headers
install: install_scripts_global
