# include the build environment
include ../common.mk

#include version information
include ./versions.mk

CONFIG_FILE ?= config.mk
include $(CONFIG_FILE)

dirs-m =
dirs-$(CONFIG_FMC)		+= fmc-bus
dirs-$(CONFIG_ZIO)		+= zio
dirs-$(CONFIG_SPEC)		+= spec-sw
dirs-$(CONFIG_SVEC)		+= svec-sw
dirs-$(CONFIG_FDELAY)		+= fine-delay-sw
dirs-$(CONFIG_OBSBOX)		+= obsbox
dirs-$(CONFIG_FMC_TDC)		+= fmc-tdc-sw

# special case for fmc-adc-100m14b4cha-sw
fmc-adc-100m14b4cha-sw-$(CONFIG_FMC_ADC) += fmc-adc-100m14b4cha-sw

.PHONY: all clean cleanall $(dirs-m) $(fmc-adc-100m14b4cha-sw-m) install

clean: TARGET = clean
cleanall: clean

# Parallel builds are not posible because multiple modules triggers building of
# dependent modules (like fmc-bus).
# When parallel build is triggered it may (and happened) that few jobs tries to
# enter/build the same module twice. In most directories it is not a problem,
# but in "kernel" directories everytime build is triggered
# "Building modules, stage 2" is executed. Which triggers errors.
all clean modules: $(fmc-adc-100m14b4cha-sw-m)
	for d in $(dirs-m); do \
		$(MAKE) -C $$d $(TARGET) || exit 1; done

# special case for fmc-adc-100m14b4cha-sw
fmc-adc-100m14b4cha-sw:
	@echo "ADC special code, we have to compile PCI and VME version"
	$(MAKE) -C fmc-adc-100m14b4cha-sw $(TARGET)
	mv fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b.ko \
	   fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-pci.ko &>/dev/null || true
	$(MAKE) CONFIG_FMC_ADC_SVEC=y -C fmc-adc-100m14b4cha-sw $(TARGET)
	mv fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b.ko \
	   fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-vme.ko &>/dev/null || true

# installation rule
install: $(addprefix install-,$(dirs-m)) install-$(fmc-adc-100m14b4cha-sw-m)

install-:

# fmc-bus
fmc-bus_DRIVERS=fmc-bus/kernel/fmc.ko
install-fmc-bus:
	$(MAKE) install_drivers_global PRODUCT_NAME=fmc-bus DRIVERS_LIST="$(fmc-bus_DRIVERS)"

# zio
zio_DRIVERS=zio/zio.ko
zio_HEADERS=zio/include/linux/zio-user.h \
	zio/include/linux/zio.h
zio_SCRIPTS=scripts/zio/install_zio.sh
zio_PROGS=zio/tools/zio-dump
install-zio: install-zio_extra_headers
	$(MAKE) install_drivers_global PRODUCT_NAME=zio DRIVERS_LIST="$(zio_DRIVERS)"
	$(MAKE) install_headers_global PRODUCT_NAME=zio HEADERS_LIST="$(zio_HEADERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=zio SCRIPTS_LIST="$(zio_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=zio PROGS_LIST="$(zio_PROGS)"

install-zio_extra_headers:
	@echo "    Create extra link in $(INST_DRIVER_PATH):"
	$(V)$(INSTALL_DIR_CMD) $(INSTALL_DIR_PARAMS) $(INST_LIB_PATH)zio/$(LIB_MAJOR).$(LIB_MINOR).$(LIB_PATCH)/include/
# create link ctrinstall -> ctrpinstall
	@echo "        linux -> zio"
	$(V)$(INSTALL_LINK) $(INSTALL_LINK_PARAMS) zio $(INST_LIB_PATH)zio/$(LIB_MAJOR).$(LIB_MINOR).$(LIB_PATCH)/include/linux


# spec-sw
spec-sw_DRIVERS=spec-sw/kernel/spec.ko
spec-sw_SCRIPTS=scripts/spec/install_spec.sh
spec-sw_PROGS=spec-sw/tools/specmem
install-spec-sw:
	$(MAKE) install_drivers_global PRODUCT_NAME=spec DRIVERS_LIST="$(spec-sw_DRIVERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=spec SCRIPTS_LIST="$(spec-sw_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=spec PROGS_LIST="$(spec-sw_PROGS)"

# svec-sw
svec-sw_DRIVERS=svec-sw/kernel/svec.ko
svec-sw_SCRIPTS=\
	scripts/svec/install_svec.sh \
	scripts/svec/svec-modparams.awk
svec-sw_PROGS=svec-sw/tools/svec-config \
	svec-sw/tools/svec-wrc-loader
install-svec-sw:
	$(MAKE) install_drivers_global PRODUCT_NAME=svec DRIVERS_LIST="$(svec-sw_DRIVERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=svec SCRIPTS_LIST="$(svec-sw_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=svec PROGS_LIST="$(svec-sw_PROGS)"

# fine-delay-sw
fine-delay-sw_DRIVERS=fine-delay-sw/kernel/fmc-fine-delay.ko
fine-delay-sw_LIBS=\
	fine-delay-sw/lib/libfdelay.a
fine-delay-sw_HEADERS=\
	fine-delay-sw/lib/fdelay-lib.h \
	fine-delay-sw/kernel/fine-delay.h
fine-delay-sw_SCRIPTS=\
	scripts/fine-delay-sw/fd-luns.awk \
	scripts/fine-delay-sw/install_fd.sh
fine-delay-sw_PROGS=fine-delay-sw/tools/fmc-fdelay-board-time \
	fine-delay-sw/tools/fmc-fdelay-input \
	fine-delay-sw/tools/fmc-fdelay-list \
	fine-delay-sw/tools/fmc-fdelay-pulse \
	fine-delay-sw/tools/fmc-fdelay-status \
	fine-delay-sw/tools/fmc-fdelay-term
install-fine-delay-sw:
	$(MAKE) install_drivers_global PRODUCT_NAME=fmc-fine-delay DRIVERS_LIST="$(fine-delay-sw_DRIVERS)"
	$(MAKE) install_libs_global    PRODUCT_NAME=fmc-fine-delay LIBS_LIST="$(fine-delay-sw_LIBS)"
	$(MAKE) install_headers_global PRODUCT_NAME=fmc-fine-delay HEADERS_LIST="$(fine-delay-sw_HEADERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=fmc-fine-delay SCRIPTS_LIST="$(fine-delay-sw_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=fmc-fine-delay PROGS_LIST="$(fine-delay-sw_PROGS)"

# obsbox
obsbox_DRIVERS=obsbox/kernel/obs-box.ko
obsbox_PROGS=obsbox/tools/obsbox-dump
install-obsbox:
	$(MAKE) install_drivers_global PRODUCT_NAME=obsbox DRIVERS_LIST="$(obsbox_DRIVERS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=obsbox PROGS_LIST="$(obsbox_PROGS)"

# fmc-tdc-sw
fmc-tdc-sw_DRIVERS=fmc-tdc-sw/kernel/fmc-tdc.ko
fmc-tdc-sw_LIBS=\
	fmc-tdc-sw/lib/libfmctdc.a
fmc-tdc-sw_HEADERS=\
	fmc-tdc-sw/lib/fmctdc-lib.h
fmc-tdc-sw_SCRIPTS=\
	scripts/fmc-tdc/install_fmc_tdc.sh
fmc-tdc-sw_PROGS=fmc-tdc-sw/tools/fmc-tdc-list \
	fmc-tdc-sw/tools/fmc-tdc-temperature \
	fmc-tdc-sw/tools/fmc-tdc-term \
	fmc-tdc-sw/tools/fmc-tdc-time \
	fmc-tdc-sw/tools/fmc-tdc-tstamp
install-fmc-tdc-sw:
	$(MAKE) install_drivers_global PRODUCT_NAME=fmc-tdc DRIVERS_LIST="$(fmc-tdc-sw_DRIVERS)"
	$(MAKE) install_libs_global    PRODUCT_NAME=fmc-tdc LIBS_LIST="$(fmc-tdc-sw_LIBS)"
	$(MAKE) install_headers_global PRODUCT_NAME=fmc-tdc HEADERS_LIST="$(fmc-tdc-sw_HEADERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=fmc-tdc SCRIPTS_LIST="$(fmc-tdc-sw_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=fmc-tdc PROGS_LIST="$(fmc-tdc-sw_PROGS)"

# fmc-adc
fmc-adc_DRIVERS=\
	fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-vme.ko \
	fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-pci.ko
fmc-adc_LIBS=\
	fmc-adc-100m14b4cha-sw/lib/libfmcadc.a
fmc-adc_HEADERS=\
	fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b4cha.h \
	fmc-adc-100m14b4cha-sw/lib/fmcadc-lib.h
fmc-adc_SCRIPTS=\
	scripts/fmc-adc-100m/install_fmc_adc_100m.sh
fmc-adc_PROGS=\
	fmc-adc-100m14b4cha-sw/libtools/fald-acq \
	fmc-adc-100m14b4cha-sw/libtools/fald-trg-cfg
install-fmc-adc-100m14b4cha-sw:
	$(MAKE) install_drivers_global PRODUCT_NAME=fmc-adc-100m DRIVERS_LIST="$(fmc-adc_DRIVERS)"
	$(MAKE) install_libs_global    PRODUCT_NAME=fmc-adc-100m LIBS_LIST="$(fmc-adc_LIBS)"
	$(MAKE) install_headers_global PRODUCT_NAME=fmc-adc-100m HEADERS_LIST="$(fmc-adc_HEADERS)"
	$(MAKE) install_scripts_global PRODUCT_NAME=fmc-adc-100m SCRIPTS_LIST="$(fmc-adc_SCRIPTS)"
	$(MAKE) install_prog_global    PRODUCT_NAME=fmc-adc-100m PROGS_LIST="$(fmc-adc_PROGS)"
