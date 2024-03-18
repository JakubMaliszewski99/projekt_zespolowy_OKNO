PROGRAMNAME = Doom style 2.5D game.exe
COMPILER = g++
CXXFLAGS = -std=c++11 -Wall
SRCDIR = src
BINDIR = bin
INCLUDEDIR = include
SFMLINCLUDEDIR = -IC:\SFML-2.5.1\include
SFMLLIBDIR = -LC:\SFML-2.5.1\lib
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(BINDIR)/$(PROGRAMNAME)

$(BINDIR)/$(PROGRAMNAME): $(BINDIR)/main.o
	$(COMPILER) -o $@ $^ $(SFMLLIBDIR) $(SFMLLIBS)
	
$(BINDIR)/main.o: $(SRCDIR)/main.cpp
	$(COMPILER) $(CXXFLAGS) -c $< -o $@ $(SFMLINCLUDEDIR)