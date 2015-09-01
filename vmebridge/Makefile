DIRS = \
	driver \
	include \
	lib \
	tools \
	test \
	drvrtest

.PHONY: all clean cleanall $(DIRS)

all clean cleanall: $(DIRS)

clean: TARGET = clean
cleanall: TARGET = cleanall

$(DIRS):
	$(MAKE) -C $@ $(TARGET)

drvrtest: driver

test: lib include

tools: lib include

lib: include
