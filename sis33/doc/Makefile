.PHONY: all clean
.SUFFIXES: .pdf .tex
.tex.pdf:
	pdflatex $*.tex >/dev/null
	pdflatex $*.tex >/dev/null
ALL=user_req.pdf sines.pdf

all: $(ALL)
clean cleanall:
	rm -f *.aux *.out *.log .log $(ALL)
