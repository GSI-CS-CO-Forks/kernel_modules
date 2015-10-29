CPU?=L866
DRVR := vmebus
INSTDIR ?= "/acc/local/$(CPU)/$(DRVR)"
include ../../common.mk
include /acc/src/dsc/co/Make.auto

all:

deliver: all
	@ umask 0002; \
	if [ ! -d $(INSTDIR) ]; then \
		echo "Creating $(INSTDIR)"; \
		mkdir -p $(INSTDIR); \
	fi; \
	echo "Copying vmebus.h and libvmebus.h to $(INSTDIR)"; \
	dsc_install vmebus.h libvmebus.h $(INSTDIR)
