# include the build environment
include ../../../common.mk

INCDIR := ../../include
CFLAGS := -Wall -g -I$(INCDIR)
LDLIBS := -lm
LIBCVORG := ../libcvorg.$(CPU).a

SRCS :=	waveform.c \
	status.c \
	outconf.c \
	inconf.c \
	freq.c \
	sequence.c \
	lock.c \
	version.c \
	dac.c \
	pcb.c \


OBJS := $(SRCS:.c=.$(CPU))

all : $(LIBCVORG) $(OBJS)

.PHONY : all clean cleanall

waveform.$(CPU):	$(@:.$(CPU)=.$c) $(LIBCVORG)
status.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
outconf.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
inconf.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
freq.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
sequence.$(CPU):	$(@:.$(CPU)=.$c) $(LIBCVORG)
lock.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
version.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
dac.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)
pcb.$(CPU):		$(@:.$(CPU)=.$c) $(LIBCVORG)

clean:
	$(RM) *.$(CPU).o
	$(RM) *.$(CPU)

cleanall:
	$(RM) *.{$(ALL_CPUS_COMMAS)}.o
	$(RM) *.{$(ALL_CPUS_COMMAS)}