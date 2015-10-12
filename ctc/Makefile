CPU?=L866
include ../common.mk
include /acc/dsc/src/co/Make.auto

DRIVER = ctc
DRIVER_NAME = "$(DRIVER)"
LIBNAME = lib$(DRIVER)

COMPILE_TIME = $(shell date +%s)
CFLAGS= -g -Wall -fPIC
CFLAGS+=-I../vmebridge/include

#used for mv
DRIVER_OBJ=ctc.o ctc.mod.o

MODLIBS = $(LIBNAME).$(CPU).a $(LIBNAME).$(CPU).so
MODLIBOBJS = $(LIBNAME).$(CPU).o

SRCS=$(LIBNAME).c $(LIBNAME).h

all: modules libs
libs: $(MODLIBS)
modules:
	$(CP) Module.symvers.vmebus Module.symvers
	$(MAKE) -C $(KERNELSRC) M=`pwd` CPU=$(CPU) GIT_VERSION=$(GIT_VERSION) 'DRIVER_NAME=$(DRIVER_NAME)' modules;
	$(MKDIR) -p $(CPU)/$(KVER)
	mv $(DRIVER_OBJ) $(CPU)/$(KVER)
	mv *.ko $(CPU)/$(KVER)
	ln -s $(CPU)/$(KVER)/$(DRIVER).ko;

clean:
	$(RM) *.$(CPU).{o,so,a}
	$(RM) -r $(CPU)
	$(RM) $(DRIVER_NAME).ko;
	$(MAKE) -C $(KERNELSRC) M=`pwd` clean;

cleanall: clean
	$(RM) *.{$(ALL_CPUS_COMMAS)}.{o,so,a}
	$(RM) -r {$(ALL_CPUS_COMMAS)}

$(LIBNAME).$(CPU).o: $(SRCS)

$(LIBNAME).$(CPU).a: $(MODLIBOBJS)
	$(AR) $(ARFLAGS) $@ $^

solibs: $(LIBNAME).$(CPU).so
$(LIBNAME).$(CPU).so: $(MODLIBOBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS) $(LDLIBS)

ACC = lab
TOOLSDIR = /acc/local/$(CPU)/drv/ctc/tools
install-test: solibs
	mkdir -p $(TOOLSDIR)
	dsc_install $(LIBNAME).$(CPU).so $(TOOLSDIR)
	install -b -m 755 ctctest $(TOOLSDIR)
	ln -sf $(TOOLSDIR)/ctctest /acc/dsc/$(ACC)/$(CPU)/bin
