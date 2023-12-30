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

class Game
{
    public:
        Game();
        ~Game();
        int play_game();

    private:
        int **real; // The contents of the board
        int rows, cols, mines;

        void populate_board();
        void print_board();
        void fill_numbers();
};

#endif
