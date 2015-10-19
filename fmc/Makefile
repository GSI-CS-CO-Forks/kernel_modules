CPU?=L866
include ../common.mk
include /acc/dsc/src/co/Make.auto

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

# By default deploy on this directory
PREFIX ?= .

DEPLOY_DRIVER ?= $(PREFIX)/$(CPU)/$(KVER)/
DEPLOY_TOOL ?= $(PREFIX)/$(CPU)/
INSTALL_OPT := -p -D

.PHONY: all clean cleanall $(dirs-m) $(fmc-adc-100m14b4cha-sw-m)

clean: TARGET = clean

#run deploy after build
all: deploy
# Parallel builds are not posible because multiple modules triggers building of
# dependent modules (like fmc-bus).
# When parallel build is triggered it may (and happened) that few jobs tries to
# enter/build the same module twice. In most directories it is not a problem,
# but in "kernel" directories everytime build is triggered
# "Building modules, stage 2" is executed. Which triggers errors.
build clean modules: $(fmc-adc-100m14b4cha-sw-m)
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

clean: clean_deploy
clean_deploy:
	$(RM) -r $(CPU)

cleanall: clean cleanall_deploy
cleanall_deploy:
	$(RM) -r $(ALL_CPUS)

# by default deploy to the $(CPU) directory
deploy: build
# Install Drivers
	install $(INSTALL_OPT) fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-pci.ko \
		$(DEPLOY_DRIVER)/fmc-adc-100m/fmc-adc-100m14b-pci.ko
	install $(INSTALL_OPT) fmc-adc-100m14b4cha-sw/kernel/fmc-adc-100m14b-vme.ko \
		$(DEPLOY_DRIVER)/fmc-adc-100m/fmc-adc-100m14b-vme.ko
	install $(INSTALL_OPT) fmc-bus/kernel/fmc.ko \
		$(DEPLOY_DRIVER)/fmc-bus/fmc.ko
	install $(INSTALL_OPT) fine-delay-sw/kernel/fmc-fine-delay.ko \
		$(DEPLOY_DRIVER)/fmc-fine-delay/fmc-fine-delay.ko
	install $(INSTALL_OPT) fmc-tdc-sw/kernel/fmc-tdc.ko \
		$(DEPLOY_DRIVER)/fmc-tdc/fmc-tdc.ko
	install $(INSTALL_OPT) spec-sw/kernel/spec.ko \
		$(DEPLOY_DRIVER)/spec/spec.ko
	install $(INSTALL_OPT) svec-sw/kernel/svec.ko \
		$(DEPLOY_DRIVER)/svec/svec.ko
	install $(INSTALL_OPT) zio/zio.ko \
		$(DEPLOY_DRIVER)/zio/zio.ko
	install $(INSTALL_OPT) obsbox/kernel/obs-box.ko \
		$(DEPLOY_DRIVER)obsbox/obs-box.ko

# Install Libraries

# Install Tools
#       FMC ADC 100M
	install $(INSTALL_OPT) fmc-adc-100m14b4cha-sw/libtools/fald-acq \
		$(DEPLOY_TOOL)/fmc-adc-100m/fald-acq
	install $(INSTALL_OPT) fmc-adc-100m14b4cha-sw/libtools/fald-trg-cfg \
		$(DEPLOY_TOOL)/fmc-adc-100m/fald-trg-cfg
#       FMC FINE DELAY
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-board-time \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-board-time
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-input \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-input
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-list \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-list
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-pulse \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-pulse
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-status \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-status
	install $(INSTALL_OPT) fine-delay-sw/tools/fmc-fdelay-term \
		$(DEPLOY_TOOL)/fmc-fine-delay/fmc-fdelay-term
#       FMC TDC
	install $(INSTALL_OPT) fmc-tdc-sw/tools/fmc-tdc-list \
		$(DEPLOY_TOOL)/fmc-tdc/fmc-tdc-list
	install $(INSTALL_OPT) fmc-tdc-sw/tools/fmc-tdc-temperature \
		$(DEPLOY_TOOL)/fmc-tdc/fmc-tdc-temperature
	install $(INSTALL_OPT) fmc-tdc-sw/tools/fmc-tdc-term \
		$(DEPLOY_TOOL)/fmc-tdc/fmc-tdc-term
	install $(INSTALL_OPT) fmc-tdc-sw/tools/fmc-tdc-time \
		$(DEPLOY_TOOL)/fmc-tdc/fmc-tdc-time
	install $(INSTALL_OPT) fmc-tdc-sw/tools/fmc-tdc-tstamp \
		$(DEPLOY_TOOL)/fmc-tdc/fmc-tdc-tstamp
#       SPEC
	install $(INSTALL_OPT) spec-sw/tools/specmem \
		$(DEPLOY_TOOL)/spec/specmem
#       SVEC
	install $(INSTALL_OPT) svec-sw/tools/svec-config \
		$(DEPLOY_TOOL)/svec/svec-config
	install $(INSTALL_OPT) svec-sw/tools/svec-wrc-loader \
		$(DEPLOY_TOOL)/svec/svec-wrc-loader
#       ZIO
	install $(INSTALL_OPT) zio/tools/zio-dump \
		$(DEPLOY_TOOL)/zio/zio-dump
#	OBSBOX
	install $(INSTALL_OPT) obsbox/tools/obsbox-dump \
		$(DEPLOY_TOOL)obsbox/tools/obsbox-dump
