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
         << "How many rows? (Must be at least 5) ";
    while (not (cin >> rows) or rows <= 4) {
        cout << "Invalid input. Please enter a positive integer (>1): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "How many columns? (Must be at least 5) ";
    while (not (cin >> cols) or cols <= 4) {
        cout << "Invalid input. Please enter a positive integer (>1): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    /* Here I suggest that 16% of the board should have mines, this is to make
     * the board equally challenging and fun.
     */
    int suggestion = rows * cols * 16 / 100;
    if (suggestion == 0) {
        suggestion++;
    }
    cout << "How many mines? (suggestion: " << suggestion << ") ";
    while (not (cin >> mines) or mines <= 0 or mines > rows * cols - 9) {
        cout << "Invalid number of mines. Please enter a positive integer "
             << "(from 1-" << rows * cols - 9 << "): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    initialize_board();
    print_board();
}

/* Game (destructor)
 *    Purpose: Frees all memory associated with the board.
 * Parameters: None
 *    Returns: Nothing
 */
Game::~Game() {
    if (real != nullptr) {
        for (int i = 0; i < rows; i++) {
            if (real[i] != nullptr) {
                delete [] real[i];
            }
        }
        delete [] real;
    }
    if (board != nullptr) {
        for (int i = 0; i < rows; i++) {
            if (board[i] != nullptr) {
                delete [] board[i];
            }
        }
        delete [] board;
    }
}

/* play_game
 *    Purpose: TODO
 * Parameters:
 *    Returns:
 */
int Game::play_game() {
    string input;
    char flag = '\0';
    int x, y;
    // First turn (selecting a starting square)
    getline(cin, input);
    while (not isValidInput(input, flag, x, y)) {
        cout << "Choose a cell in the format \"" << RED << "x " << BLUE
             << "y" << RESET << "\": ";
        getline(cin, input);
    }
    populate_board(x - 1, y - 1);
    for (int i = x - 2; i < x + 1; i++) {
        if (i >= 0 and i < rows) {
            for (int j = y - 2; j < y + 1; j++) {
                if (j >= 0 and j < cols) {
                    board[i][j] = true;
                }
            }
        }
    }
    board[x - 1][y - 1] = true;
    // Command Loop for after the first turn
    string status = "keep going";
    while (status != EXPLOSION and status != WIN) {
        print_board();
        cout << "Choose a cell in the format \"[f] " << RED << "x " << BLUE
             << "y" << RESET << "\" (f = set a flag): ";
        getline(cin, input);
        while (not isValidInput(input, flag, x, y)) {
            cout << "Choose a cell in the format \"[f] " << RED << "x " << BLUE
                 << "y" << RESET << "\" (f = set a flag): ";
            getline(cin, input);
        }
        board[x - 1][y - 1] = true;
        status = check_board();
    }
    return end_game(status);
}

int Game::end_game(string &status) {
    if (status == EXPLOSION) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (real[i][j] == -1) {
                    board[i][j] = true;
                }
            }
        }
        print_board();
        cout << endl << "BOOOM!!!! Game over.\n";
        return 0;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = true;
        }
    }
    print_board();
    cout << endl << "Congratulations! You won!\n";
    return 1;
}

/* initialize_board
 *    Purpose: Create memory for the board values
 * Parameters: None
 *    Returns: Nothing
 */
void Game::initialize_board() {
    board = new bool * [rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new bool [cols];
        for (int j = 0; j < cols; j++) {
            board[i][j] = false;
        }
    }
    real = new int * [rows];
    for (int i = 0; i < rows; i++) {
        real[i] = new int [cols];
        for (int j = 0; j < cols; j++) {
            real[i][j] = 0;
        }
    }
}

/* populate_board
 *    Purpose: Takes the user's inputted grid dimensions and number of mines,
 *             and builds the board for them to play on.
 * Parameters: Integers x and y, representing the first coordinates the user
 *             selects so that the game can start from that point and the board
 *             is built around it.
 *    Returns: Nothing
 */
void Game::populate_board(int x, int y) {
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
        // First selected square can't have a mine, neither can the ones around
        if (real[randRow][randCol] == 0
            and not ((randCol == y and
                        (randRow == x or randRow == x - 1 or randRow == x + 1))
                     or (randCol == y + 1 and (randRow == x
                                               or randRow == x - 1
                                               or randRow == x + 1))
                     or (randCol == y - 1 and (randRow == x
                                               or randRow == x - 1
                                               or randRow == x + 1)))) {
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
void Game::fill_numbers() {
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
void Game::print_board() {
    cout << endl;
    for (int i = 0; i < rows; i++) {
        // Print the x-axis of the board
        cout << BLUE << i + 1 << RESET;
        if (i >= 9) {
            cout << " | ";
        } else {
            cout << "  | ";
        }
        // Print the cells on the board
        for (int j = 0; j < cols; j++) {
            /* The following conditional determines whether a cell should be
             * revealed or not.
             */
            if (not board[i][j]) {
                cout << DARK_GREY << CELL << "  " << RESET;
            } else {
                if (real[i][j] == -1) {
                    cout << MINE << " " << RESET;
                } else if (real[i][j] == -2) {
                    cout << BOOM << " " << RESET;
                } else {
                    color_num(real[i][j]);
                }
            }
        }
        cout << endl;
    }
    // Print the x-axis of the board
    cout << "    ";
    for (int i = 0; i < cols; i++) {
        cout << "---";
    }
    cout << endl << "     ";
    for (int i = 0; i < cols; i++) {
        if (i >= 8) {
            cout << RED << i + 1 << " " << RESET;
        } else {
            cout << RED << i + 1 << "  " << RESET;
        }
    }
    cout << endl;
}

/* color_num
 *    Purpose: Prints out the revealed numbers on the board with the correct
 *             colorings.
 * Parameters: An integer n representing the number to be printed.
 *    Returns: Nothing
 */
void Game::color_num(int n) {
    if (n == 0) {
        cout << "   ";
        return;
    } else if (n == 1) {
        cout << BLUE;
    } else if (n == 2) {
        cout << GREEN;
    } else if (n == 3) {
        cout << RED;
    } else if (n == 4) {
        cout << DARK_BLUE;
    } else if (n == 5) {
        cout << MAROON;
    } else if (n == 6) {
        cout << CYAN;
    } else if (n == 7) {
        cout << YELLOW;
    } else if (n == 8) {
        cout << MAGENTA;
    }
    cout << n << "  " << RESET;
}

string Game::check_board() {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j]) {
                if (real[i][j] == -1)  {
                    real[i][j] = -2;
                    return EXPLOSION;
                }
                count++;
            }
        }
    }
    if (count == rows * cols - mines) {
        return WIN;
    }
    return "keep going";
}

bool Game::isValidInput(string &input, char &flag, int &x, int &y) {
    stringstream ss(input);
    char firstChar;
    ss >> firstChar;
    if (isdigit(firstChar)) {
        ss.clear();
        ss.seekg(0);
    } else {
        flag = firstChar;
    }
    if (ss >> y >> x) {
        if ((flag == 'f' or flag == '\0') and
            (x > 0 and y > 0 and x <= rows and y <= cols)) {
            return true;
        }
    }
    return false;
}
