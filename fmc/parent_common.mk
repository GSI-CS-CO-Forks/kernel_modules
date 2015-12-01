REPO_PARENT ?= $(shell pwd)
include $(REPO_PARENT)/../common.mk

#overwrite rules from Make.auto
%: %.o
	$(LINK.o) $^  $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

%: %.c
	$(LINK.c) $^  $(LOADLIBES) $(LDLIBS) -o $@

LINUX=$(KERNELSRC)
export LINUX
export CC
export LD
export CROSS_COMPILE


FMC_BUS ?= $(REPO_PARENT)/fmc-bus
export FMC_BUS
ZIO ?= $(REPO_PARENT)/zio
export ZIO
SPEC_SW ?= $(REPO_PARENT)/spec-sw
export SPEC_SW
SVEC_SW ?= $(REPO_PARENT)/svec-sw
export SVEC_SW

CONFIG_SUPER_REPO ?= coht
CONFIG_SUPER_REPO_VERSION ?= $(shell cd $(REPO_PARENT); git describe --always --dirty --long --tags)
export CONFIG_SUPER_REPO
export CONFIG_SUPER_REPO_VERSION
