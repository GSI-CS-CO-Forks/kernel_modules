# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

all: driver
driver: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)
	mv Module.symvers $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)


DRIVERS_DEPLOY=vmebus.ko

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global

HEADERS_LIST=vmebus.h

# use default instalation rule for headers
install: install_headers_global

install: install_extra_headers

install_extra_headers:
# create link acc/local/$(CPU)/include/vmebus.h -> ../drv/$(PRODUCT_NAME)/vmebus.h
	@echo "    Create link acc/local/$(CPU)/include/vmebus.h -> ../drv/$(PRODUCT_NAME)/vmebus.h "
	$(V)$(INSTALL_DIR_CMD) $(INSTALL_DIR_PARAMS) $(DEPLOY_PATH)acc/local/$(CPU)/include
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) ../drv/$(PRODUCT_NAME)/vmebus.h $(DEPLOY_PATH)acc/local/$(CPU)/include/vmebus.h
