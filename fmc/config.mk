#
# Use CONFIG_ macros, even if we are not yet using Kconfig. Use "m", not "y".
# Most of these can be set to =n in the environment to disable building
# the respective module

CONFIG_FMC = m
CONFIG_ZIO = m
CONFIG_SPEC ?= m
CONFIG_SVEC ?= m
CONFIG_FDELAY ?= m
CONFIG_FMC_ADC ?= m
CONFIG_OBSBOX ?= m
CONFIG_FMC_TDC ?= m
CONFIG_ADC_LIB ?=m
