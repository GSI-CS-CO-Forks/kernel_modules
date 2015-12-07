.PHONY: all clean
.SUFFIXES: .pdf .tex
.tex.pdf:
	pdflatex $*.tex &>/dev/null
	pdflatex $*.tex &>/dev/null

all: user_req.pdf sines.pdf
clean:
	rm -f *.aux *.out *.log .log *.pdf
