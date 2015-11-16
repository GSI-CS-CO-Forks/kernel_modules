CPU ?= L866

# ALL_CPUS has to be at least two elements due to brace expansion
ALL_CPUS=L865 L866 L867

ifeq ($(CPU),L865)
KVER ?= 3.2.43-rt63
endif

ifeq ($(CPU),L866)
KVER ?= 3.6.11-rt30.25.el6rt.x86_64
endif

ifeq ($(CPU),L867)
KVER ?= 3.10.0-229.1.2.rt56.141.2.el7_1.x86_64
endif

GIT_VERSION = $(shell git describe --dirty --long --tags --always)

KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)

# used to convert list separated with spaces into list separated with commas
comma:= ,
empty:=
space:= $(empty) $(empty)
#used for removals
ALL_CPUS_COMMAS=$(subst $(space),$(comma),$(ALL_CPUS))

# use bash due to brace expansion
SHELL=/bin/bash
