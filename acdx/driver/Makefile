CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS=-g -Wall

all: driver 
driver: modules

modules: 
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(TOOLS) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	$(RM) -r $(CPU)
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean

cleanall: clean
	$(RM) -r $(ALL_CPUS)
