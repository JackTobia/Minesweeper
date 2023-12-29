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

using namespace std;

class Game
{
    public:
        Game();
        // TODO: play function, with command loop and print board

    private:
        vector<vector<char> > real;
        int rows, cols, mines;
        void populate_board();
};

#endif
