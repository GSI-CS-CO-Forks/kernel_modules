# include the build environment
include ../common.mk

DRIVER = ctc
DRIVER_NAME = "$(DRIVER)"
LIBNAME = lib$(DRIVER)

COMPILE_TIME = $(shell date +%s)
CFLAGS= -g -Wall -fPIC
CFLAGS+=-I../vmebridge/include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

#used for mv
DRIVER_OBJ=ctc.o ctc.mod.o

MODLIBS = $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so
MODLIBOBJS = $(LIBNAME).$(CPU).o

SRCS=$(LIBNAME).c $(LIBNAME).h

all: modules libs examples
libs: $(MODLIBS)
modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) 'DRIVER_NAME=$(DRIVER_NAME)' CROSS_COMPILE=$(TOOLS) modules;
	$(MKDIR) -p $(CPU)/$(KVER)
	mv $(DRIVER_OBJ) $(CPU)/$(KVER)
	mv *.ko $(CPU)/$(KVER)
	ln -s $(CPU)/$(KVER)/$(DRIVER).ko;

clean:
	$(RM) *.$(CPU).{o,so,a}
	$(RM) -r $(CPU)
	$(RM) $(DRIVER_NAME).ko;
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean;
	$(MAKE) -C test CPU=$(CPU) clean

cleanall: clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) -r {$(ALL_CPUS_COMMAS)}
	$(MAKE) -C test CPU=$(CPU) cleanall

$(LIBNAME).$(CPU).o: $(SRCS)

$(LIBNAME).$(CPU).a: $(MODLIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

solibs: $(LIBNAME).$(CPU).so
$(LIBNAME).$(CPU).so: $(MODLIBOBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

examples: libs
	$(MAKE) -C test CPU=$(CPU)

ACC = lab
TOOLSDIR = /acc/local/$(CPU)/drv/ctc/tools
install-test: solibs
	mkdir -p $(TOOLSDIR)
	dsc_install $(LIBNAME).$(CPU).so $(TOOLSDIR)
	install -b -m 755 ctctest $(TOOLSDIR)
	ln -sf $(TOOLSDIR)/ctctest /acc/dsc/$(ACC)/$(CPU)/bin
