all: docs

docs: Doxyfile.patch
	doxygen -s -g
	patch -p0 < Doxyfile.patch
	doxygen 2>&1
	rm Doxyfile

clean cleanall:
	rm -rf html Doxyfile
