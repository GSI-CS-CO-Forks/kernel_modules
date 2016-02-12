# include the build environment
include ../common.mk

CFLAGS+=-I../vmebridge/include
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

all: vmeio.$(CPU)

clean:
	$(RM) vmeio.$(CPU)

cleanall:
	$(RM) vmeio.{$(ALL_CPUS_COMMAS)}

PRODUCT_NAME=rawio

PROGS_LIST=vmeio.$(CPU)

# version of the programs/tools
PROG_MAJOR=1
PROG_MINOR=0
PROG_PATCH=0

# use default instalation rule for programs/tools
install: install_prog_global

# define versions
VER_PREV=
VER_CURRENT=$(PROG_MAJOR).$(PROG_MINOR)
VER_NEXT=
