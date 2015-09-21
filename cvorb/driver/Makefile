CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

OUTPUTDIR=$(CURDIR)/$(CPU)/$(KVER)

CFLAGS=-g -Wall -Wstrict-prototypessss
all: driver 
driver: modules

# compile with V=1 to see compilation details
modules: 
	cp Module.symvers.vme Module.symvers
	make -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules
	mkdir -p $(OUTPUTDIR)
	mv *.o *.ko $(OUTPUTDIR)
clean:
	rm -f *.o *.a *.pyc *.so $(TESTS)
	rm -rf L865 L864 ppc 
	make -C $(KERNELSRC) M=`pwd` clean


