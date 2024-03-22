PROGRAMNAME = doom.out
COMPILER = g++
CXXFLAGS = -std=c++11 -Wall
SRCDIR = src
BINDIR = bin
INCLUDEDIR = include
SFMLINCLUDEDIR = -IC:\SFML-2.5.1\include
SFMLLIBDIR = -LC:\SFML-2.5.1\lib
SFMLLIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)%.cpp,$(BINDIR)%.o,$(SRCS))

all: $(BINDIR)/$(PROGRAMNAME)

$(BINDIR)/$(PROGRAMNAME): $(OBJS)
	$(COMPILER) -o $@ $^ $(SFMLLIBDIR) $(SFMLLIBS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(COMPILER) $(CXXFLAGS) -c $< -o $@ $(SFMLINCLUDEDIR)

clear:
	rm -rf $(BINDIR)/$(PROGRAMNAME) $(BINDIR)/*.o