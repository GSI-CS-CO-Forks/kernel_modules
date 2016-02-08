# include the build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS += -g -Wall
CFLAGS += -I../driver
CFLAGS += -I../../pciioconfig/lib
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS+= -L../../pciioconfig/lib
LDLIBS += -lpciioconfig.$(CPU)

all: test_driver.$(CPU)

clean:
	$(RM) test_driver.$(CPU)

cleanall:
	$(RM) test_driver.{$(ALL_CPUS_COMMAS)}

PROGS_LIST=test_driver.$(CPU)

install: install_prog_global
