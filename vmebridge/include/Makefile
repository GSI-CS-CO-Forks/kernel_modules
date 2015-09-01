CPU?=L866
DRVR := vmebus
INSTDIR ?= "/acc/local/$(CPU)/$(DRVR)"
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all: vmebus.h libvmebus.h

vmebus.h:
	ln -s ../driver/vmebus.h vmebus.h

libvmebus.h:
	ln -s ../lib/libvmebus.h libvmebus.h

.PHONY: all clean deliver

clean cleanall:
	$(RM) -f vmebus.h libvmebus.h

deliver: all
	@ umask 0002; \
	if [ ! -d $(INSTDIR) ]; then \
		echo "Creating $(INSTDIR)"; \
		mkdir -p $(INSTDIR); \
	fi; \
	echo "Copying vmebus.h and libvmebus.h to $(INSTDIR)"; \
	dsc_install vmebus.h libvmebus.h $(INSTDIR)

