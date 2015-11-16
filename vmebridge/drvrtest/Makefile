#
# Makefile for test drivers.
#
CPU?=L866
# include the build environment
include ../../common.mk

OUTPUTDIR?=`pwd`/$(CPU)/$(KVER)

all: vmedriver vmeheaders mymodules

.PHONY : all vmedriver vmeheaders mymodules clean help

vmedriver:
	cp ../driver/$(CPU)/$(KVER)/Module.symvers .

vmeheaders:
	$(MAKE) -C ../include

mymodules: vmedriver
	$(MAKE) -C $(KERNELSRC) M=`pwd` CROSS_COMPILE=$(TOOLS) modules
	mkdir -p $(OUTPUTDIR)
	mv *.o *.ko $(OUTPUTDIR)

help:
	$(MAKE) -C $(KERNELSRC) M=`pwd` help

clean cleanall:
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean
	rm -rf $(ALL_CPUS)
