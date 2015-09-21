CPU?=L866
include ../../common.mk
include /acc/dsc/src/co/Make.auto

CFLAGS=-g -Wall -Wstrict-prototypessss
all: driver
driver: modules

# compile with V=1 to see compilation details
modules: 
	$(CP) Module.symvers.vme Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) modules
	$(MKDIR) -p $(CPU)/$(KVER)
	mv *.o *.ko $(CPU)/$(KVER)

clean:
	rm -f *.o *.a *.pyc *.so $(TESTS)
	rm -rf L865 L864 ppc 
	make -C $(KERNELSRC) M=`pwd` clean


