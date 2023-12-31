/*
 *      Game.h
 *      Minesweeper
 *      Project 2
 *      by Jack Tobia, 12/29/2023
 *
 *      A header file for the Game class, which will hold the game board and
 *      all functionality necessary for the game. TODO
 *
 */

#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <random>

using namespace std;

// ANSI color codes for text formatting
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
// #define GREEN   "\033[32m"
// #define YELLOW  "\033[33m"
// #define MAGENTA "\033[35m"
// #define CYAN    "\033[36m"
// #define WHITE   "\033[37m"

class Game
{
    public:
        Game();
        ~Game();
        int play_game();

    private:
        int **real; // The contents of the board
        int **board;
        int rows, cols, mines;

        void populate_board();
        void print_board();
        void fill_numbers();
};

#endif
