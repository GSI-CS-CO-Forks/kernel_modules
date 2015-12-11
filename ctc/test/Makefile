# include build environment
include ../../common.mk

#include version information
include ../versions.mk

CFLAGS=-g -Wall

CFLAGS += -I../
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

LDLIBS=../libctc.$(CPU).a
all: set_clk.$(CPU) set_delay_counter1.$(CPU) set_delay_counter2.$(CPU) set_mode.$(CPU) set_enable.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}

PROGS_LIST=\
	set_clk.$(CPU) \
	set_delay_counter1.$(CPU) \
	set_delay_counter2.$(CPU) \
	set_mode.$(CPU) \
	set_enable.$(CPU) \

install: install_prog_global
