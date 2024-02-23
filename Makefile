PROGRAMNAME = FPS
COMPILER = g++
CXXFLAGS = -std=c++11 -Wall
SRCDIR = src
BINDIR = bin
INCLUDEDIR = include
SFMLINCLUDEDIR = -I.\include
SFMLLIBDIR = -L.\lib
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(BINDIR)/main.exe

$(BINDIR)/main.exe: $(BINDIR)/main.o $(BINDIR)/Game.o
	$(COMPILER) -o $@ $^ $(SFMLLIBDIR) $(SFMLLIBS)
	
$(BINDIR)/main.o: $(SRCDIR)/main.cpp $(INCLUDEDIR)/Game.h
	$(COMPILER) $(CXXFLAGS) -c $< -o $@ $(SFMLINCLUDEDIR)
	
$(BINDIR)/Game.o: $(SRCDIR)/Game.cpp $(INCLUDEDIR)/Game.h
	$(COMPILER) $(CXXFLAGS) -c $< -o $@ $(SFMLINCLUDEDIR)