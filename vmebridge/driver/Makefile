include ../../common.mk
OUTPUTDIR?=`pwd`/$(CPU)/$(KVER)

CFLAGS= -g -Wall

all: driver
driver: modules

modules:
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) modules
	mkdir -p $(OUTPUTDIR)
	mv *.o *.ko $(OUTPUTDIR)
	mv Module.symvers $(OUTPUTDIR)

clean cleanall:
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean
	rm -rf $(ALL_CPUS)
