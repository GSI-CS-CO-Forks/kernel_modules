# Makefile for building the Kernel-Space Device Driver

CPU ?= L865
ENV_PREFIX=/acc/sys/$(CPU)

# include /acc/src/dsc/co/Make.auto

ifeq ($(CPU),L866)
    KVER ?= 3.6.11.2-rt33.39.el6rt.x86_64
endif
ifeq ($(CPU),L865)
    KVER ?= 3.2.43-rt63
endif
ifeq ($(CPU),L864)
    KVER ?= 2.6.9-34
endif

KDIR = $(ENV_PREFIX)/usr/src/kernels/$(KVER)

ifndef ACCS
    ACCS=lab oper oplhc
endif

LIB_SRCS=pickeringmuxlib.c
DRV_SRCS=pickeringmuxmain.c pickering16to16.c pickering16to16a.c pickering4to1.c pickering9to8.c pickering22to8.c pickeringattn.c
TST_SRCS=test_driver.c
ccflags-y += -g -Wall -I. -I/acc/local/$(CPU)/include
MY_LDFLAGS= -g

obj-m += pickeringmuxdrv.o
pickeringmuxdrv-objs := $(DRV_SRCS:.c=.o)

PWD := $(shell pwd)

all:    libpickeringmux.$(CPU).a test_driver.$(CPU) driver

libpickeringmux.$(CPU).a: $(LIB_SRCS:.c=.$(CPU).o)
	@rm -f $@
	ar crv $@ $^
	ranlib $@


test_driver.$(CPU): $(TST_SRCS:.c=.$(CPU).o) libpickeringmux.$(CPU).a
	@rm -f $@ /tmp/$@
	$(CC) $(MY_LDFLAGS) $^  -L/acc/local/$(CPU)/lib -lpciioconfig -o /tmp/$@
	mv /tmp/$@ $@

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install: install_driver install_lib

install_driver: $(obj-m:.o=.ko) pickmuxinstall
	for a in $(ACCS); do \
	    dsc_install $^ /acc/dsc/$$a/$(CPU)/$(KERNELVERSION)/pickeringmux; \
	done

install_lib: libpickeringmux.$(CPU).a  pickeringmuxlib.h
	dsc_install $^ /acc/local/$(CPU)/pickering


clean:
	rm -f *~ core .*.cmd *.o *.ko -r .tmp* *.cmd.? .es* ,*
