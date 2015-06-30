# Makefile for building the Kernel-Space Device Driver

CPU ?= L865

GIT_VERSION = $(shell git describe --tags --dirty --always)

include /acc/src/dsc/co/Make.auto

ifeq ($(CPU),L866)
    KVER ?= 3.6.11-rt30.25.el6rt.x86_64
endif
ifeq ($(CPU),L865)
    KVER ?= 3.2.43-rt63
endif
ifeq ($(CPU),L864)
    KVER ?= 2.6.9-34
endif

KDIR = /acc/sys/$(CPU)/usr/src/kernels/$(KVER)

ifndef ACCS
    ACCS=lab oper oplhc
endif

CFLAGS += -g -Wall
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS += -g

all:     driver libpickeringmux.$(CPU).a test_driver.$(CPU)

libpickeringmux.$(CPU).a: pickeringmuxlib.$(CPU).o
	@rm -f $@
	ar crv $@ $^
	ranlib $@

test_driver.$(CPU): test_driver.$(CPU).o libpickeringmux.$(CPU).a
	@rm -f $@ /tmp/$@
	$(CC) $(LDFLAGS) $^  -L/acc/local/$(CPU)/lib -lpciioconfig -o /tmp/$@
	mv /tmp/$@ $@

driver:
	$(MAKE) -C $(KDIR) M=$(PWD) GIT_VERSION=$(GIT_VERSION) modules

install: install_driver install_lib

install_driver: pickeringmuxdrv.$(CPU).ko pickmuxinstall
	for a in $(ACCS); do \
	    dsc_install $^ /acc/dsc/$$a/$(CPU)/$(KERNELVERSION)/pickeringmux; \
	done

install_lib: libpickeringmux.$(CPU).a  pickeringmuxlib.h
	dsc_install $^ /acc/local/$(CPU)/pickering

doc:
	sh makedoc
doc-clean:
	rm -rf html Doxyfile Doxyfile.bak
doc-install: doc
	rsync -av html/ /acc/doc/html/private/coht/doxy/pickeringmux

clean: doc-clean
	rm -rf *~ core .*.cmd *.o *.ko .tmp* *.cmd.? \
		.es* ,* *.a *.mod.c *.$(CPU) \
		Module.symvers modules.order
