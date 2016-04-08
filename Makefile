include common.mk
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
DIRS-$(CONFIG_pickering-lxi) += pickering-lxi
DIRS-$(CONFIG_pickeringmux) += pickeringmux
DIRS-$(CONFIG_rawio) += rawio
DIRS-$(CONFIG_sis33) += sis33
DIRS-$(CONFIG_vd80-2) += vd80-2
DIRS-$(CONFIG_vmebridge) += vmebridge
DIRS-$(CONFIG_vmod) += vmod
DIRS-$(CONFIG_soft-cpu-toolchains) += soft-cpu-toolchains
DIRS-$(CONFIG_fip) += fip

.PHONY: all clean cleanall install $(DIRS-y) init_submodules init_submodules_post init_submodules_pre

all clean cleanall install: $(DIRS-y)
clean: TARGET = clean
cleanall: TARGET = cleanall
install: TARGET = install

$(DIRS-y):
	$(MAKE) -C $@ $(TARGET)

# dependencies
pickeringmux: pciioconfig

jtag: vmebridge

# ctc needs encore
ctc: encore

# cvora needs encore
cvora: encore


# init_submodules rule is used by CI machines (or by any other user) to be able
# to clone submodules from ohwr.org on machines without internet connecton.
# NOTE: It removes existing submodules!
init_submodules: init_submodules_post

# Define which machine to use to get submodules
GIT_GATEWAY=cs-ccr-builds

# Define all submodules that shall be cloned. To simplify we don't distinguish
# between CERN and outside repos.
# We cannot simply parse all .gitmodules, because for fmc's repos we don't want
# to clone recursively all submodules (for example we keep only one copy of
# fmc-bus).
# Nested submodules are supported, but not used.
SUBMODULE_LIST+='(mil1553		"ssh://git@gitlab.cern.ch:7999/cohtdrivers/mil1553.git")'
SUBMODULE_LIST+='(ctr-2.0		"ssh://git@gitlab.cern.ch:7999/cohtdrivers/ctr-2.0.git")'
SUBMODULE_LIST+='(encore		"ssh://git@gitlab.cern.ch:7999/cohtdrivers/encore.git")'
SUBMODULE_LIST+='(fmc/adc-lib		"git://ohwr.org/misc/adc-lib.git")'
SUBMODULE_LIST+='(fmc/zio		"git://ohwr.org/misc/zio.git")'
SUBMODULE_LIST+='(fmc/fmc-bus		"git://ohwr.org/fmc-projects/fmc-bus.git")'
SUBMODULE_LIST+='(fmc/spec-sw		"git://ohwr.org/fmc-projects/spec/spec-sw.git")'
SUBMODULE_LIST+='(fmc/svec-sw		"git://ohwr.org/fmc-projects/svec/svec-sw.git")'
SUBMODULE_LIST+='(fmc/fine-delay-sw	"git://ohwr.org/fmc-projects/fmc-delay-1ns-8cha/fine-delay-sw.git")'
SUBMODULE_LIST+='(fmc/fmc-adc-100m14b4cha-sw "git://ohwr.org/fmc-projects/fmc-adc-100m14b4cha/fmc-adc-100m14b4cha-sw.git")'
SUBMODULE_LIST+='(fmc/fmc-tdc-sw	"git://ohwr.org/fmc-projects/fmc-tdc/fmc-tdc-sw.git")'
SUBMODULE_LIST+='(fmc/obsbox		"ssh://git@gitlab.cern.ch:7999/cohtdrivers/obsbox.git")'
SUBMODULE_LIST+='(ipack			"ssh://git@gitlab.cern.ch:7999/cohtdrivers/ipack.git")'
SUBMODULE_LIST+='(mtt			"ssh://git@gitlab.cern.ch:7999/cohtdrivers/mtt.git")'
SUBMODULE_LIST+='(ctc			"ssh://git@gitlab.cern.ch:7999/cohtdrivers/ctc.git")'
SUBMODULE_LIST+='(cvora			"ssh://git@gitlab.cern.ch:7999/cohtdrivers/cvora.git")'
SUBMODULE_LIST+='(soft-cpu-toolchains	"git://ohwr.org/misc/soft-cpu-toolchains.git")'
SUBMODULE_LIST+='(fip			"gitlab.cern.ch:7999/cohtdrivers/fip.git")'

init_submodules_pre:
#	initialize submodules (non recursive)
	git submodule init

init_submodules_all: init_submodules_pre
#	Create tempdir and save its name
	$(eval TEMP_OHWR_DIR := $(shell ssh $(GIT_GATEWAY) "sh -c \"mktemp -d ohwr.XXXXXXXXXX --tmpdir\""))
	@echo "Using temporary directory $(TEMP_OHWR_DIR) on git gateway ($(GIT_GATEWAY))"
#	Clone submodules on GIT_GATEWAY and init submodules (needed for nested submodules).
#	Please note that we remove repo from the place where we want to copy new one.
#	If we don't remove it rsync may/will mess up in .git
	-for d in $(SUBMODULE_LIST); do \
	    declare -a arrayofrepos=$$d;\
	    path=$${arrayofrepos[0]};\
	    url=$${arrayofrepos[1]};\
	    echo "Removing $$path if present";\
	    rm -rf $$path;\
	    echo "Cloning repo $$url to $$path";\
	    ssh $(GIT_GATEWAY) "cd $(TEMP_OHWR_DIR) && git clone -- \"$$url\" \"$$path\" && cd \"$$path\" && git submodule init" \
	    || exit 1; \
	    done

#	Transfer all cloned repos to the local machine
	-rsync -a $(GIT_GATEWAY):$(TEMP_OHWR_DIR)/* .
#	remove temporary directory on $(GIT_GATEWAY)
	ssh $(GIT_GATEWAY) "rm -rf $(TEMP_OHWR_DIR)"

init_submodules_post: init_submodules_all
#	Checkout correct commit in copied repos
	for d in $(SUBMODULE_LIST); do \
	    declare -a arrayofrepos=$$d;\
	    path=$${arrayofrepos[0]};\
	    git submodule update --checkout \
	    || exit 1; \
	    done
