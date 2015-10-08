include /acc/src/dsc/co/Make.auto

all: doc

clean cleanall: doc-clean

doc:
	sh makedoc

doc-clean:
	$(RM) -r html Doxyfile*

install: doc
	rsync -av html/ /acc/doc/html/private/coht/doxy/pickeringmux
