PROGRAMNAME = FPS
COMPILER = g++
SRCDIR = src
BINDIR = bin
SFMLINCLUDEDIR = -I.\include
SFMLLIBDIR = -L.\lib
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(BINDIR)/main.exe

$(BINDIR)/main.exe: $(BINDIR)/main.o
	$(COMPILER) -o $(BINDIR)/main.exe $(BINDIR)/main.o $(SFMLLIBDIR) $(SFMLLIBS)
	
$(BINDIR)/main.o: $(SRCDIR)/main.cpp
	$(COMPILER) -std=c++11 -Wall -c $(SRCDIR)/main.cpp -o $(BINDIR)/main.o $(SFMLINCLUDEDIR)