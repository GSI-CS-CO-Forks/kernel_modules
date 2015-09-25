DIRS = \
    driver \
    lib \
    test \
    doc \


.PHONY: all clean cleanall install $(DIRS)

all clean cleanall install: $(DIRS)
clean: TARGET = clean
cleanall: TARGET = cleanall
install: TARGET = install

$(DIRS):
	$(MAKE) -C $@ $(TARGET)
