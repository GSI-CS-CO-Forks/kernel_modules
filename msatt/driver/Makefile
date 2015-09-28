CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
