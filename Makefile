
%.pdf : %.fig
	fig2dev -L pdf $< > $@

DOCNAME = linear-regression
TEXNAME = $(DOCNAME).tex
PDFNAME = $(DOCNAME).pdf

.PHONY : all clean

all : $(PDFNAME)

$(PDFNAME) : $(TEXNAME) logo.pdf fdl-1.3.tex
	pdflatex $(TEXNAME)
	pdflatex $(TEXNAME)

clean :
	@rm -fv *.aux
	@rm -fv *.log
	@rm -fv *.out
	@rm -fv logo.pdf
	@rm -fv $(PDFNAME)

