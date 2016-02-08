# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS+= -I../include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: libvmebus.$(CPU).a

libvmebus.$(CPU).a: libvmebus.$(CPU).o
	$(AR) rv $@ $^

libvmebus.$(CPU).o: libvmebus.c libvmebus.h

clean:
	$(RM) *.$(CPU).{o,a}

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,a}

LIBS_LIST=libvmebus.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

HEADERS_LIST=libvmebus.h

# use default instalation rule for headers
install: install_headers_global

install: install_extra_lib install_extra_headers

install_extra_headers:
# create link acc/local/$(CPU)/include/libvmebus.h -> ../drv/$(PRODUCT_NAME)/libvmebus.h
	@echo "    Create link acc/local/$(CPU)/include/libvmebus.h -> ../drv/$(PRODUCT_NAME)/libvmebus.h "
	$(V)$(INSTALL_DIR_CMD) $(INSTALL_DIR_PARAMS) $(DEPLOY_PATH)acc/local/$(CPU)/include
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) ../drv/$(PRODUCT_NAME)/libvmebus.h $(DEPLOY_PATH)acc/local/$(CPU)/include/libvmebus.h

install_extra_lib:
# create link acc/local/$(CPU)/include/libvmebus.a -> ../drv/$(PRODUCT_NAME)/libvmebus.a
	@echo "    Create link acc/local/$(CPU)/lib/libvmebus.a -> ../drv/$(PRODUCT_NAME)/libvmebus.a "
	$(V)$(INSTALL_DIR_CMD) $(INSTALL_DIR_PARAMS) $(DEPLOY_PATH)acc/local/$(CPU)/lib
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) ../drv/$(PRODUCT_NAME)/libvmebus.a $(DEPLOY_PATH)acc/local/$(CPU)/lib/libvmebus.a
