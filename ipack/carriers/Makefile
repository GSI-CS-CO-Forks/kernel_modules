CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

all: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
