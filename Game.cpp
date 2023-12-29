/*
 *      Game.cpp
 *      Minesweeper
 *      Project 2
 *      by Jack Tobia, 12/29/2023
 *
 *      An implementation for the class Game, including... TODO
 *
 */

#include "Game.h"

/* Game (constructor)
 *    Purpose: Prompts the user to specify the dimensions of the board, as
 *             well as the number of bombs they would like to have in the
 *             board.
 * Parameters: None
 *    Returns: Nothing
 */
Game::Game()
{
    cout << "Welcome to Minesweeper!\nBefore we begin, let's make a board!\n"
         << "How many rows? ";
    while (not (cin >> rows) || rows <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "How many columns? ";
    while (not (cin >> cols) || cols <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    /* Here I suggest that 16% of the board should have bombs, this is to make
     * the board equally challenging and fun.
     */
    cout << "How many bombs? (suggestion: " << rows * cols * 16 / 100 << ") ";
    while (not (cin >> mines) || mines <= 0 || mines >= rows * cols) {
        cout << "Invalid number of bombs. Please enter a positive integer "
             << "(from 1-" << rows * cols - 1 << "): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

}

/* populate_board
 *    Purpose:
 * Parameters:
 *    Returns:
 */
void Game::populate_board()
{
    // TODO
    return;
}
