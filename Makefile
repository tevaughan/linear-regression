
# Copyright 2016  Thomas E. Vaughan
#
# The present software is redistributable under the terms of the GNU LESSER
# GENERAL PUBLIC LICENSE, which is must be distributed in the file, 'LICENSE',
# along with the software.

# --- BEG Automatic dependencies for C and C++ files. ---
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
SRCS=$(shell echo *.cpp)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d
%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d
# --- END Automatic dependencies for C and C++ files. ---

CXXFLAGS = -g -O0 -std=c++11 -Wall
CPPFLAGS = -I/usr/include/eigen3
LDLIBS   = -lm

PROGRAMS = sinusoid
PROG_PDF = $(PROGRAMS:=.pdf)

%.pdf : %.gpi %.dat
	gnuplot $<

%.dat : %
	./$< > $@

%.pdf : %.fig
	fig2dev -L pdf $< > $@

DOCNAME = linear-regression
TEXNAME = $(DOCNAME).tex
PDFNAME = $(DOCNAME).pdf

.PHONY : all clean

all : $(PDFNAME)

$(PDFNAME) : $(TEXNAME) logo.pdf fdl-1.3.tex $(PROGRAMS) $(PROG_PDF)
	pdflatex $(TEXNAME)
	pdflatex $(TEXNAME)

sinusoid : sinusoid.o basis.o
	$(CXX) -o $@ $^ $(LDLIBS)

clean :
	@rm -frv .d
	@rm -fv *.aux
	@rm -fv *.log
	@rm -fv logo.pdf
	@rm -fv *.o
	@rm -fv *.out
	@rm -fv $(PDFNAME)
	@rm -fv $(PROG_PDF)
	@rm -fv $(PROGRAMS)

# This must be the last line.
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
