DIRS = \
    driver \
    lib \
    test \

.PHONY: all clean cleanall $(DIRS)

all clean cleanall: $(DIRS)
clean: TARGET = clean
cleanall: TARGET = cleanall

$(DIRS):
	$(MAKE) -C $@ $(TARGET)

test: lib
