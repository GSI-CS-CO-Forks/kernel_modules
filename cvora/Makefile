# include the build environment
include ../common.mk

#include version information
include ./versions.mk

all: modules libs

#used for mv
DRIVER_OBJ=cvora.o cvora.mod.o

modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` GIT_VERSION=$(GIT_VERSION) CROSS_COMPILE=$(CROSS_COMPILE) modules;
	$(MKDIR) -p $(CPU)/$(KVER)
	mv $(DRIVER_OBJ) $(CPU)/$(KVER)
	mv *.ko $(CPU)/$(KVER)
	ln -s $(CPU)/$(KVER)/cvora.ko

clean:
	$(RM) *.$(CPU).{o,so,a}
	$(RM) -r $(CPU)
	$(RM) cvora.ko;
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean;

cleanall: clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) -r {$(ALL_CPUS_COMMAS)}
	$(MAKE) -C doc clean

docs:
	$(MAKE) -C doc

CFLAGS= -g -Wall -fPIC
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"


libs: libcvora.$(CPU).so libcvora.$(CPU).a

libcvora.$(CPU).o: libcvora.c libcvora.h
libcvora.$(CPU).so: libcvora.$(CPU).o
	$(CC) $(CFLAGS) -shared -o $@ $^
libcvora.$(CPU).a: libcvora.$(CPU).o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

DRVRPATH = /acc/dsc/$(ACC)/$(CPU)/$(KVER)/cvora
LIBPATH = /acc/local/$(CPU)/drv/cvora

install: install-test

install-test:
	$(MAKE) -C test install

LIBS_LIST=libcvora.$(CPU).a

# use default instalation rule for libs
install: install_libs_global

# deploy shared library together with programs
PROGS_LIST=libcvora.$(CPU).so

install: install_prog_global

HEADERS_LIST=libcvora.h

#use default instalation rule for headers
install: install_headers_global

DRIVERS_DEPLOY=cvora.ko

# add the path to the drivers ($(CPU)/$(KVER))
DRIVERS_LIST=$(addprefix $(CPU)/$(KVER)/,$(DRIVERS_DEPLOY))

install: install_drivers_global

SCRIPTS_LIST=\
	install_cvora.sh \
	transfer2insmod.awk \

# use default instalation rule for headers
install: install_scripts_global
