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
 *             well as the number of mines they would like to have in the
 *             board. This information is used to set up the board.
 * Parameters: None
 *    Returns: Nothing
 */
Game::Game()
{
    cout << "Welcome to Minesweeper!\nBefore we begin, let's make a board!\n"
         << "How many rows? ";
    while (not (cin >> rows) || rows <= 1) {
        cout << "Invalid input. Please enter a positive integer (>1): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "How many columns? ";
    while (not (cin >> cols) || cols <= 1) {
        cout << "Invalid input. Please enter a positive integer (>1): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    /* Here I suggest that 16% of the board should have bombs, this is to make
     * the board equally challenging and fun.
     */
    int suggestion = rows * cols * 16 / 100;
    if (suggestion == 0) {
        suggestion++;
    }
    cout << "How many mines? (suggestion: " << suggestion << ") ";
    while (not (cin >> mines) || mines <= 0 || mines >= rows * cols) {
        cout << "Invalid number of mines. Please enter a positive integer "
             << "(from 1-" << rows * cols - 1 << "): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    populate_board();
    print_board();
}

/* Game (destructor)
 *    Purpose: Frees all memory associated with the board.
 * Parameters: None
 *    Returns: Nothing
 */
Game::~Game()
{
    for (int i = 0; i < rows; i++) {
        delete [] real[i];
    }
    delete [] real;
}

/* play_game
 *    Purpose:
 * Parameters:
 *    Returns:
 */
int Game::play_game()
{
    // TODO
    return 0;
}

/* populate_board
 *    Purpose: Takes the user's inputted grid dimensions and number of mines,
 *             and builds the board for them to play on.
 * Parameters: None
 *    Returns: Nothing
 */
void Game::populate_board()
{
    real = new int * [rows];
    for (int i = 0; i < rows; i++) {
        real[i] = new int [cols];
        for (int j = 0; j < cols; j++) {
            real[i][j] = 0;
        }
    }

    // Generate random mine positions by setting those positions to -1
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> disRows(0, rows - 1);
    uniform_int_distribution<int> disCols(0, cols - 1);

    // Placing mines
    int placed = 0;
    while (placed < mines) {
        int randRow = disRows(gen);
        int randCol = disCols(gen);
        if (real[randRow][randCol] == 0) {
            real[randRow][randCol] = -1;
            placed++;
        }
    }

    fill_numbers();
}

/* fill_numbers
 *    Purpose: Takes the board filled with mines, and gives number values to
 *             non-mine spots in the board which represent the number of mines
 *             that surround the cell.
 * Parameters: None
 *    Returns: Nothing
 */
void Game::fill_numbers()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (real[i][j] != -1) {
                if (i != 0) {
                    if (j != 0) {
                        real[i][j] += (real[i - 1][j - 1] == -1);
                    }
                    real[i][j] += (real[i - 1][j] == -1);
                    if (j != cols - 1) {
                        real[i][j] += (real[i - 1][j + 1] == -1);
                    }
                }
                if (j != 0) {
                    real[i][j] += (real[i][j - 1] == -1);
                    if (i != rows - 1) {
                        real[i][j] += (real[i + 1][j - 1] == -1);
                    }
                }
                if (i != rows - 1) {
                    real[i][j] += (real[i + 1][j] == -1);
                    if (j != cols - 1) {
                        real[i][j] += (real[i + 1][j + 1] == -1);
                    }
                }
                if (j != cols - 1) {
                    real[i][j] += (real[i][j + 1] == -1);
                }
            }
        }
    }
}

/* print_board
 *    Purpose: Prints out the current state of the board
 * Parameters: None
 *    Returns: Nothing
 */
void Game::print_board()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (real[i][j] == -1) {
                cout << "_ ";
            } else {
                cout << real[i][j] << " ";
            }
        }
        cout << endl;
    }
}
