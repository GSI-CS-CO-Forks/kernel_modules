# include the build environment
include ../common.mk

DIRS = \
    doc \
    drivers \
    include \
    lib \
    scripts \
    test \

.PHONY: all clean cleanall install $(DIRS)

all clean cleanall install: $(DIRS)
clean: TARGET = clean
cleanall: TARGET = cleanall
install: TARGET = install

$(DIRS):
	$(MAKE) -C $@ $(TARGET)

test: lib
