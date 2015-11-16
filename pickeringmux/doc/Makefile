
all: doc

clean cleanall: doc-clean

doc:
	sh makedoc

doc-clean:
	rm -rf html Doxyfile*

install: doc
	rsync -av html/ /acc/doc/html/private/coht/doxy/pickeringmux
