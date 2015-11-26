# include the build environment
include ../../common.mk

# Flags to compile test program and lib
CFLAGS=-Wall -g
CFLAGS+=-I../lib
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS+=-L../lib
LDLIBS+=-lpciioconfig.$(CPU)

all: test_pciioconfig.$(CPU)

clean:
	$(RM) test_pciioconfig.$(CPU)

cleanall:
	$(RM) test_pciioconfig.{$(ALL_CPUS_COMMAS)}
