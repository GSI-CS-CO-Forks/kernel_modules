CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

.PHONY: all clean cleanall

all: modules

modules:
	cp Module.symvers.vme Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CPU=$(CPU) modules
	$(MKDIR) -p ./$(CPU)/$(KVER)
	mv *.o *.ko ./$(CPU)/$(KVER)

clean:
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean
	$(RM) -r $(CPU)

cleanall: clean
	$(RM) -r $(ALL_CPUS)
