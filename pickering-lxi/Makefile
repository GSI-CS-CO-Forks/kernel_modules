################################################################################
# Makefile for compile ClientBridge modules and example applications
#
# Copyright (C) 2013 Pickering Interfaces ltd.
# 
# Author: Michal Heczko
# Contact: michal.heczko@pickeringmail.cz
# Version: 2.1.0
#
################################################################################

# build machinery
#
# include the build environment
include ../common.mk

CXXFLAGS+= -g -Wall -fPIC
CXXFLAGS+=-DGIT_VERSION=\"$(GIT_VERSION)\"

# standard deployment paths
LIBROOT ?= /acc/local/$(CPU)/drv/pickering-lxi/$(DISTRO_VERSION)
LIBDIR ?= $(LIBROOT)/lib
INCSYSDIR ?= $(LIBROOT)/include/pickering-lxi

# Distribution version
DISTRO_VERSION=1.10.3

# Specify C++ compiler
GG=$(CXX) $(CPPFLAGS) $(CXXFLAGS)

# General
CURDIR=`pwd`
MACHINE=$(shell hostname)
BINDIR=bin
OBJSDIR=$(BINDIR)/objects
DOCDIR=Docs
INCOPT=-I./src/Picmlx -I./src/Piplx -I./src/Pipslx -I./src/Pi743lx
LINKOPTS=-lpthread -ldl
CFLAGS_MODULES_DEBUG=

# Picmlx - Communication module
PICMLX_VERSION=$(shell grep "\#define *PICMLX_MODULE_VERSION" src/Picmlx/Picmlx.cpp | sed 's/\r$$//'\
 | sed 's/[ \t]*[ \t]/ /g' | cut -f3 -d' '\
 | awk -F "" '{for(i=1;i<=NF;i++){printf(".%d",$$i)}}' | sed 's/.//')
PICMLX=picmlx
PICMLXSRCDIR=./src/Picmlx
PICMLXOBJS=$(OBJSDIR)/BaseList.cpp.o $(OBJSDIR)/Sockets.cpp.o $(OBJSDIR)/System.cpp.o $(OBJSDIR)/Session.cpp.o \
    $(OBJSDIR)/PicmlxModule.cpp.o $(OBJSDIR)/Picmlx.cpp.o $(OBJSDIR)/DebugTool.cpp.o
ifeq "$(MACHINE)" "warpdevel"
CFLAGS_PICMLX=-I$(PICMLXSRCDIR) $(INCOPT) -D_REENTRANT -DLINUX -Wno-psabi
else
CFLAGS_PICMLX=-I$(PICMLXSRCDIR) $(INCOPT) -D_REENTRANT -DLINUX
endif

# Piplx - Pickering switching module
PIPLX_VERSION=$(shell grep "\#define *PIPLX_MODULE_VERSION" src/Piplx/Piplx.cpp | sed 's/\r$$//'\
 | sed 's/[ \t]*[ \t]/ /g' | cut -f3 -d' '\
 | awk -F "" '{for(i=1;i<=NF;i++){printf(".%d",$$i)}}' | sed 's/.//')
PIPLX=piplx
PIPLXSRCDIR=./src/Piplx
PIPLXOBJS=$(OBJSDIR)/PiplxModule.cpp.o $(OBJSDIR)/Piplx.cpp.o
CFLAGS_PIPLX=-I$(PIPLXSRCDIR) $(INCOPT) -D_REENTRANT -DLINUX

# Piplx - Pickering switching module
PIPSLX_VERSION=$(shell grep "\#define *PIPSLX_MODULE_VERSION" src/Pipslx/Pipslx.cpp | sed 's/\r$$//'\
 | sed 's/[ \t]*[ \t]/ /g' | cut -f3 -d' '\
 | awk -F "" '{for(i=1;i<=NF;i++){printf(".%d",$$i)}}' | sed 's/.//')
PIPSLX=pipslx
PIPSLXSRCDIR=./src/Pipslx
PIPSLXOBJS=$(OBJSDIR)/PipslxModule.cpp.o $(OBJSDIR)/Pipslx.cpp.o 
CFLAGS_PIPSLX=-I$(PIPSLXSRCDIR) $(INCOPT) -D_REENTRANT -DLINUX

# Pi743lx - pi743 PSU module
PI743LX_VERSION=$(shell grep "\#define *PI743LX_MODULE_VERSION" src/Pi743lx/Pi743lx.cpp | sed 's/\r$$//'\
 | sed 's/[ \t]*[ \t]/ /g' | cut -f3 -d' '\
 | awk -F "" '{for(i=1;i<=NF;i++){printf(".%d",$$i)}}' | sed 's/.//')
PI743LX=pi743lx
PI743LXSRCDIR=./src/Pi743lx
PI743LXOBJS=$(OBJSDIR)/Pi743lxModule.cpp.o $(OBJSDIR)/Pi743lx.cpp.o 
CFLAGS_PI743LX=-I$(PI743LXSRCDIR) $(INCOPT) -D_REENTRANT -DLINUX

# Check if debug mode is requied
ifeq ($(debug),1)
	CFLAGS_MODULES_DEBUG=-D_DEBUG	-DDEBUG
endif

# Check if it's ported to ARM architecture
ifeq ($(arm),1)	
	CFLAGS_MODULES_DEBUG+=-D_STRONGARM -DSTRONGARM
endif

# Check if debug file is requied
ifeq ($(debug_file),1)
	CFLAGS_MODULES_DEBUG+=-DDEBUGFILE=$(debug_file)
endif

CFLAGS_PICMLX+=$(CFLAGS_MODULES_DEBUG)
CFLAGS_PIPLX+=$(CFLAGS_MODULES_DEBUG)
CFLAGS_PIPSLX+=$(CFLAGS_MODULES_DEBUG)
CFLAGS_PI743LX+=$(CFLAGS_MODULES_DEBUG)

#############################################################################

all: setup picmlx piplx pipslx pi743lx

# 'setup' sets up the build directory structure the way we want
setup:	
	@if test ! -d $(BINDIR) ; then $(MKDIR) -p $(BINDIR) ; fi
	@if test ! -d $(OBJSDIR) ; then $(MKDIR) -p $(OBJSDIR) ; fi

$(LIBDIR) $(INCSYSDIR):
	mkdir -p $@

install: $(LIBDIR) $(INCSYSDIR)
	cp $(BINDIR)/lib$(PICMLX).so.$(PICMLX_VERSION) $(LIBDIR)
	ln -s $(LIBDIR)/lib$(PICMLX).so.$(PICMLX_VERSION) $(LIBDIR)/lib$(PICMLX).so
	cp $(BINDIR)/lib$(PIPLX).so.$(PIPLX_VERSION) $(LIBDIR)
	ln -s $(LIBDIR)/lib$(PIPLX).so.$(PIPLX_VERSION) $(LIBDIR)/lib$(PIPLX).so
	cp $(BINDIR)/lib$(PIPSLX).so.$(PIPSLX_VERSION) $(LIBDIR)
	ln -s $(LIBDIR)/lib$(PIPSLX).so.$(PIPSLX_VERSION) $(LIBDIR)/lib$(PIPSLX).so
	cp $(BINDIR)/lib$(PI743LX).so.$(PI743LX_VERSION) $(LIBDIR)
	ln -s $(LIBDIR)/lib$(PI743LX).so.$(PI743LX_VERSION) $(LIBDIR)/lib$(PI743LX).so
	@if test ! -d $(INCSYSDIR) ; then mkdir $(INCSYSDIR) ; fi
	cp $(PICMLXSRCDIR)/Picmlx.h $(INCSYSDIR)
	cp $(PIPLXSRCDIR)/Piplx.h $(INCSYSDIR)
	cp $(PIPSLXSRCDIR)/Pipslx.h $(INCSYSDIR)
	cp $(PI743LXSRCDIR)/Pi743lx.h $(INCSYSDIR)

uninstall:
	rm -f $(LIBDIR)/libpi*lx.so*
	rm -f $(INCSYSDIR)/Pi*lx.h

# 'clean' removes targets and objects
clean:
	@if test -d $(BINDIR) ; then rm -r $(BINDIR) ; fi

#############################################################################

picmlx: setup $(PICMLXOBJS)
	$(GG) -shared -o $(BINDIR)/lib$(PICMLX).so.$(PICMLX_VERSION) $(PICMLXOBJS) $(LINKOPTS)
	ln -sfv $(CURDIR)/$(BINDIR)/lib$(PICMLX).so.$(PICMLX_VERSION) $(CURDIR)/$(BINDIR)/lib$(PICMLX).so	
	@echo "Picmlx module v".$(PICMLX_VERSION)." done."
	@echo " "

$(OBJSDIR)/%.cpp.o: $(PICMLXSRCDIR)/%.cpp
	$(GG) $(CFLAGS_PICMLX) -c -o $@ $<

$(OBJSDIR)/%.cpp.o: $(SRCDIR)/%.cpp
	$(GG) $(CFLAGS_PICMLX) -c -o $@ $<
	
#############################################################################

piplx: setup $(PIPLXOBJS)
	$(GG) -shared -o $(BINDIR)/lib$(PIPLX).so.$(PIPLX_VERSION) $(PIPLXOBJS) $(LINKOPTS) -l$(PICMLX) -L$(CURDIR)/$(BINDIR)
	ln -sfv $(CURDIR)/$(BINDIR)/lib$(PIPLX).so.$(PIPLX_VERSION) $(CURDIR)/$(BINDIR)/lib$(PIPLX).so
	@echo "Piplx module v".$(PIPLX_VERSION)." done."
	@echo " "

$(OBJSDIR)/%.cpp.o: $(PIPLXSRCDIR)/%.cpp
	$(GG) $(CFLAGS_PIPLX) -fPIC -c -o $@ $<
	
#############################################################################

pipslx: setup $(PIPSLXOBJS)
	$(GG) -shared -o $(BINDIR)/lib$(PIPSLX).so.$(PIPSLX_VERSION) $(PIPSLXOBJS) $(LINKOPTS) -l$(PICMLX) -L$(CURDIR)/$(BINDIR)
	ln -sfv $(CURDIR)/$(BINDIR)/lib$(PIPSLX).so.$(PIPSLX_VERSION) $(CURDIR)/$(BINDIR)/lib$(PIPSLX).so
	@echo "Pipslx module v".$(PIPSLX_VERSION)." done."
	@echo " "

$(OBJSDIR)/%.cpp.o: $(PIPSLXSRCDIR)/%.cpp
	$(GG) $(CFLAGS_PIPSLX) -fPIC -c -o $@ $<

#############################################################################

pi743lx: setup $(PI743LXOBJS)
	$(GG) -shared -o $(BINDIR)/lib$(PI743LX).so.$(PI743LX_VERSION) $(PI743LXOBJS) $(LINKOPTS) -l$(PICMLX) -L$(CURDIR)/$(BINDIR)
	ln -sfv $(CURDIR)/$(BINDIR)/lib$(PI743LX).so.$(PI743LX_VERSION) $(CURDIR)/$(BINDIR)/lib$(PI743LX).so
	@echo "Pi743lx module v".$(PI743LX_VERSION)." done."
	@echo " "

$(OBJSDIR)/%.cpp.o: $(PI743LXSRCDIR)/%.cpp
	$(GG) $(CFLAGS_PI743LX) -fPIC -c -o $@ $<

#############################################################################

