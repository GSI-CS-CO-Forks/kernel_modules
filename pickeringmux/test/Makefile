CPU ?= L866
include ../../common.mk
include /acc/src/dsc/co/Make.auto

CFLAGS += -g -Wall
CFLAGS += -I../driver
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
LDLIBS += -lpciioconfig

all: test_driver.$(CPU)

clean:
	$(RM) test_driver.$(CPU)

cleanall:
	$(RM) test_driver.{$(ALL_CPUS_COMMAS)}

install:
