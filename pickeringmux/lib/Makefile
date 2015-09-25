CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

ifndef ACCS
    ACCS=lab oper oplhc
endif

CFLAGS += -g -Wall
CFLAGS += -I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: libpickeringmux.$(CPU).a

libpickeringmux.$(CPU).a: pickeringmuxlib.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

install: install_lib

install_lib: libpickeringmux.$(CPU).a  pickeringmuxlib.h
	dsc_install $^ /acc/local/$(CPU)/pickering

clean:
	$(RM) libpickeringmux.$(CPU).a
	$(RM) pickeringmuxlib.$(CPU).o

cleanall:
	$(RM) libpickeringmux.{$(ALL_CPUS_COMMAS)}.a
	$(RM) pickeringmuxlib.{$(ALL_CPUS_COMMAS)}.o
