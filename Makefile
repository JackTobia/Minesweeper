###
### Makefile for Minesweeper Project
###
### Author:  Jack Tobia


CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow

# This rule builds the executable Minesweeper
Minesweeper: minesweeper.o Game.o
	${CXX} -o Minesweeper minesweeper.o Game.o

# This rule builds minesweeper.o
minesweeper.o: minesweeper.cpp Game.h
	$(CXX) $(CXXFLAGS) -c minesweeper.cpp

# This rule builds Game.o
Game.o: Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp
