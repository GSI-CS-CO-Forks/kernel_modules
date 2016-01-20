# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

# empty rules
all clean cleanall:

SCRIPTS_LIST=\
	install.native \
	setup.acdx \

# use default instalation rule for headers
install: install_scripts_global

install: install_links

install_links:
	@echo "    Create extra links in $(INST_DRIVER_PATH):"
	$(V)$(INSTALL_DIR_CMD) $(INSTALL_DIR_PARAMS) $(INST_DRIVER_PATH)
# create link install -> .
	@echo "        install -> ."
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) . $(INST_DRIVER_PATH)/install
# create link to the test program since setup.acdx requires it: test -> ../../../../../../acc/local/L866/drv/acdx
	@echo "        test -> ../../../../../../$(INST_LIB_PATH_RELATIVE)"
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) ../../../../../../$(INST_LIB_PATH_RELATIVE) $(INST_DRIVER_PATH)/test
