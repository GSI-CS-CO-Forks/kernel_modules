DIRS = \
    sis33 \
    vd80-2 \
    encore \
    mil1553 \
    ctr-2.0 \
    cvorb \
    cvorg \
    vmebridge \
    cvora \
    pickeringmux \

.PHONY: all clean cleanall $(DIRS)

all clean cleanall: $(DIRS)
clean: TARGET = clean
cleanall: TARGET = cleanall

$(DIRS):
	$(MAKE) -C $@ $(TARGET)

