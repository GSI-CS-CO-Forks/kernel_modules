# If it exists includes Makefile.specific. In this Makefile, you should put
# specific Makefile code that you want to run before this. For example,
# build a particular environment.
-include Makefile.specific

LIBVME = ../lib

CFLAGS = -Wall -I. -I../include --static $(EXTRACFLAGS)
LDFLAGS = -L$(LIBVME) -lvmebus

progs := vme

all: $(progs)

%: %.c $(lib)
	$(CC) $(CFLAGS) $*.c $(LDFLAGS) -o $@

.PHONY: all clean

clean:
	$(RM) $(progs) *.o
