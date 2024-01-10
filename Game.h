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
#include <sstream>

using namespace std;

// ANSI color codes for text formatting
#define RESET     "\033[0m"
#define BLUE      "\033[34m"
#define GREEN     "\033[32m"
#define RED       "\033[31m"
#define DARK_BLUE "\033[34;2m"
#define MAROON    "\033[0;31;2m"
#define CYAN      "\033[36m"
#define YELLOW    "\033[33m"
#define MAGENTA   "\033[35m"
#define BLACK     "\033[0;30m"
#define DARK_GREY "\033[1;30m"

#define FLAG "\u2691"
#define CELL "\u25A0"
#define MINE "\U0001F4A3"
#define BOOM "\U0001F4A5"

#define EXPLOSION   "BOOOOM!!!!"
#define WIN         "chicken dinner"

class Game
{
    public:
        Game();
        ~Game();
        int play_game();

    private:
        struct Cell {
            int val;            // Cell's value (bomb or number)
            bool rev;           // True if cell is revealed to user
            bool empty_check;   // True if empty cell has been checked
        };

        Cell **board;
        int rows, cols, mines;

        void initialize_board();
        void populate_board(int x, int y);
        void print_board();
        void fill_numbers();
        void color_num(int n);
        string check_board();
        void recurse_reveal(int x, int y);
        bool isValidInput(string &input, char &flag, int &x, int &y);
        int end_game(string &status);
};

#endif
