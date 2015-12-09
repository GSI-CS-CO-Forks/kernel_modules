DOXY_INSTPATH = /acc/doc/html/private/coht/doxy/vd80-2.0

all:
	@echo Make documentation for linux VD80 library
	doxygen Doxyfile &>/dev/null

clean cleanall:
	rm -rf doc

doxy_install: all
	mkdir -p $(DOXY_INSTPATH)
	cp -r doc/html/* $(DOXY_INSTPATH)/

# empty install rule
install:
