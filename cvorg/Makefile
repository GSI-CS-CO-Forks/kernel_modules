# include the build environment
include ../common.mk

DIRS = \
    ad9516 \
    driver \
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

test: lib ad9516
