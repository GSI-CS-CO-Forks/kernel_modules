all: docs

docs: Doxyfile.patch
	doxygen -s -g
	patch -p0 < Doxyfile.patch
	doxygen
	rm Doxyfile

clean cleanall:
	rm -rf html Doxyfile
