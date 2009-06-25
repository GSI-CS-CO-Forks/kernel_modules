###############################################################################
# @file Makefile.tst
#
# @brief Builds up test programs.
#
# @author Yury GEORGIEVSKIY. CERN BE/CO
#
# @date Created on 13/01/2009
###############################################################################

# Makefile from current directory supress one from upper level
-include $(shell if [ -e ./Makefile.specific ]; then \
		echo ./Makefile.specific; \
	else \
		echo ../Makefile.specific; \
	fi)

-include ../../makefiles/Makefile.base

INSTDIR    = $(addsuffix /$(DLVRYPLS)/$(CPU), $(EXECINSTDIR))
ADDCFLAGS  = $(STDFLAGS) -DDRIVER_NAME=\"$(DRIVER_NAME)\"

# libraries (and their pathes) to link executable file with
XTRALIBDIRS = ../../utils/user ../$(FINAL_DEST)
LOADLIBES  := $(addprefix -L,$(XTRALIBDIRS)) $(LOADLIBES) -lutils.$(CPU) \
		-lxml2 -lz -ltermcap

# Get all local libs (in object_ directory) user wants to compile with
LOCAL_LIBS = $(patsubst ../$(FINAL_DEST)/lib%.a, -l%, $(wildcard ../$(FINAL_DEST)/*.$(CPU).a))
XTRALIBS   =
LDLIBS     = \
	   $(LOCAL_LIBS) \
	   $(XTRALIBS)

vpath %.c ./  ../../utils/user ../../utils/extest

SRCFILES = $(wildcard *.c)

# the standard test program (utils/extest) will be compiled
# unless USE_EXTEST is set to 'n'
ifneq ($(USE_EXTEST), n)
SRCFILES += \
	extest.c \
	lite_shell.c

# if the driver is skel, we'll compile in all the skel handlers
ifeq ($(IS_SKEL), y)
SRCFILES += cmd_skel.c
else
# if not, then the generic ones are taken to handle built-in commands
SRCFILES += cmd_generic.c
endif

endif
# end USE_EXTEST

ifeq ($(CPU), ppc4)
SRCFILES    += extra_for_lynx.c
else
LOADLIBES += -lrt
endif

INCDIRS = \
	./ \
	../.. \
	../driver \
	../include \
	../../utils \
	../../utils/user \
	../../include \
	../../utils/extest


ADDINCLUDES = $(KERN_INCLUDES)

EXEC_OBJS = $(DRIVER_NAME)Tst.$(CPU)

$(EXEC_OBJS): $(OBJFILES)

build:: abort $(FINAL_DEST) $(OBJDIR) $(EXEC_OBJS) move_objs ../$(FINAL_DEST)/testprog.$(CPU)

# Move compiled files to proper place
move_objs:
	mv $(OBJFILES) $(OBJDIR)

# we should redefine Make.auto rule to get rid of name dependencies
%.$(CPU): $(OBJFILES)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

../$(FINAL_DEST)/testprog.$(CPU):
	@if [ -e "$$@" ]; then \
		rm -f $@ ; \
	echo "ln -s ../test/$(EXEC_OBJS) $@"; \
	ln -s  ../test/$(EXEC_OBJS) $@; \
	fi

install:: abort
	$(CP) -p $(EXEC_OBJS) $(INSTDIR)/$(BSP)/$(FINAL_DEST)
	ln -fs ../$(BSP)/$(FINAL_DEST)/$(DRIVER_NAME)Test $(INSTDIR)/drvrutil/$(DRIVER_NAME)Test
	ln -fs ../drvrutil/$(DRIVER_NAME)Test $(INSTDIR)/bin/$(DRIVER_NAME)Test

cleanloc clearloc:: abort
	@ if [ -n "$(OBJDIR)" ]; then \
		rm -rf $(OBJDIR)* ; \
	fi
	-rm -f ../$(FINAL_DEST)/testprog $(DRIVER_NAME)Tst
	-find ./ -name '*~' -o -name '*.$(CPU).o' | xargs rm -f
