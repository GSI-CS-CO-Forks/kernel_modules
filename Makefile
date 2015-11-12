CONFIG_FILE ?= config.mk
include $(CONFIG_FILE)

DIRS-y =
DIRS-$(CONFIG_acdx) += acdx
DIRS-$(CONFIG_ctc) += ctc
DIRS-$(CONFIG_ctr-2.0) += ctr-2.0
DIRS-$(CONFIG_cvora) += cvora
DIRS-$(CONFIG_cvorb) += cvorb
DIRS-$(CONFIG_cvorg) += cvorg
DIRS-$(CONFIG_encore) += encore
DIRS-$(CONFIG_fmc) += fmc
DIRS-$(CONFIG_icv196) += icv196
DIRS-$(CONFIG_ipack) += ipack
DIRS-$(CONFIG_jtag) += jtag
DIRS-$(CONFIG_mil1553) += mil1553
DIRS-$(CONFIG_msatt) += msatt
DIRS-$(CONFIG_mtt) += mtt
DIRS-$(CONFIG_pciioconfig) += pciioconfig
DIRS-$(CONFIG_pickeringmux) += pickeringmux
DIRS-$(CONFIG_rawio) += rawio
DIRS-$(CONFIG_sis33) += sis33
DIRS-$(CONFIG_vd80-2) += vd80-2
DIRS-$(CONFIG_vmebridge) += vmebridge
DIRS-$(CONFIG_vmod) += vmod

.PHONY: all clean cleanall $(DIRS-y)

all clean cleanall: $(DIRS-y)
clean: TARGET = clean
cleanall: TARGET = cleanall

$(DIRS-y):
	$(MAKE) -C $@ $(TARGET)
