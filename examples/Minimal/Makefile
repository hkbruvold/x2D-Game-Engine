# Simple Makefile to compile on Linux

NAME=minimal

OUTDIR=bin
SRCDIR=Source

CXX=g++
LD=$(CXX)
RM=rm -f


INCDIR=../../include

CPPFLAGS=-Wall -I$(INCDIR)
LDFLAGS=

SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(subst .cpp,.o,$(SOURCES))

$(OUTDIR)/$(NAME): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/$(NAME) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean
clean: 
	$(RM) $(SRCDIR)*.o
