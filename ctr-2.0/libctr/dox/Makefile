DOXY_INSTPATH = /acc/doc/html/private/coht/doxy/libctr

all:
	echo Make documentation for libctr shared object library
	./doxy.sh -n"libctr Library" -o "doc" libctr.doxygen

doxy_install: all
	mkdir -p $(DOXY_INSTPATH)
	cp -r doc/html/* $(DOXY_INSTPATH)
