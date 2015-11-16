# include build environment
include ../../common.mk

CFLAGS=-g -Wall

CFLAGS += -I../
LDLIBS=../libctc.$(CPU).a
all: set_clk.$(CPU) set_delay_counter1.$(CPU) set_delay_counter2.$(CPU) set_mode.$(CPU) set_enable.$(CPU)

clean:
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}
