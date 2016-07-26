#CPU?=L867
CPU?=x86_64
#include ../../common.mk
#include /acc/dsc/src/co/Make.auto

all: driver
driver: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules
	mkdir -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)
	mv Module.symvers $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)

