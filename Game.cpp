/*
 *      Game.cpp
 *      Minesweeper
 *      Project 2
 *      by Jack Tobia, 12/29/2023
 *
 *      An implementation for the Game class.
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
        cout << "Invalid input. Please enter a positive integer (>4): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "How many columns? (Must be at least 5) ";
    while (not (cin >> cols) or cols <= 4) {
        cout << "Invalid input. Please enter a positive integer (>4): ";
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

/* ~Game (destructor)
 *    Purpose: Frees all memory associated with the board.
 * Parameters: None
 *    Returns: Nothing
 */
Game::~Game() {
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
 *    Purpose: The command loop for the Minesweeper game.
 * Parameters: None
 *    Returns: An integer, which is 1 if the user wins the game, 0 otherwise.
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
                    board[i][j].rev = true;
                }
            }
        }
    }
    string status = check_board();
    // Command Loop for after the first turn
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
        if (flag == 'f') {
            // Flags can only be placed on unrevealed cells
            if (not board[x - 1][y - 1].rev) {
                board[x - 1][y - 1].flag = not board[x - 1][y - 1].flag;
            }
        } else if (not board[x - 1][y - 1].flag) {
            board[x - 1][y - 1].rev = true;
        }
        status = check_board();
        flag = '\0';
    }
    return end_game(status);
}

/* end_game
 *    Purpose: Messages to indicate to the user whether they won or lost the
 *             game after a game-ending move. This includes revealing all mine
 *             locations.
 * Parameters: A string representing the way in which the game is ending.
 *    Returns: An integer, which is 1 if the user wins the game, 0 otherwise.
 */
int Game::end_game(string &status) {
    if (status == EXPLOSION) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j].val == -1) {
                    board[i][j].rev = true;
                }
            }
        }
        print_board();
        cout << endl << "BOOOM!!!! Game over, you lose.\n";
        return 0;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].rev = true;
        }
    }
    print_board();
    cout << endl << "Congratulations! You won!\n";
    return 1;
}

/* initialize_board
 *    Purpose: Create memory for the board Cells.
 * Parameters: None
 *    Returns: Nothing
 */
void Game::initialize_board() {
    board = new Cell * [rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new Cell [cols];
        for (int j = 0; j < cols; j++) {
            board[i][j].rev = false;
            board[i][j].empty_check = false;
            board[i][j].flag = false;
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
        if (board[randRow][randCol].val == 0
            and not ((randCol == y and
                        (randRow == x or randRow == x - 1 or randRow == x + 1))
                     or (randCol == y + 1 and (randRow == x
                                               or randRow == x - 1
                                               or randRow == x + 1))
                     or (randCol == y - 1 and (randRow == x
                                               or randRow == x - 1
                                               or randRow == x + 1)))) {
            board[randRow][randCol].val = -1;
            placed++;
        }
    }
    fill_numbers();
}

/* fill_numbers
 *    Purpose: Takes the board filled with mines, and gives number values to
 *             non-mine spots in the board which represent the number of mines
 *             that surround the Cell.
 * Parameters: None
 *    Returns: Nothing
 */
void Game::fill_numbers() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].val != -1) {
                if (i != 0) {
                    if (j != 0) {
                        board[i][j].val += (board[i - 1][j - 1].val == -1);
                    }
                    board[i][j].val += (board[i - 1][j].val == -1);
                    if (j != cols - 1) {
                        board[i][j].val += (board[i - 1][j + 1].val == -1);
                    }
                }
                if (j != 0) {
                    board[i][j].val += (board[i][j - 1].val == -1);
                    if (i != rows - 1) {
                        board[i][j].val += (board[i + 1][j - 1].val == -1);
                    }
                }
                if (i != rows - 1) {
                    board[i][j].val += (board[i + 1][j].val == -1);
                    if (j != cols - 1) {
                        board[i][j].val += (board[i + 1][j + 1].val == -1);
                    }
                }
                if (j != cols - 1) {
                    board[i][j].val += (board[i][j + 1].val == -1);
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
    int flags_used = 0;
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
             * revealed to the user or not.
             */
            if (board[i][j].flag) {
                cout << RED << FLAG << "  " << RESET;
                flags_used++;
            } else if (not board[i][j].rev) {
                cout << DARK_GREY << CELL << "  " << RESET;
            } else {
                if (board[i][j].val == -1) {
                    cout << MINE << " " << RESET;
                } else if (board[i][j].val == -2) {
                    cout << BOOM << " " << RESET;
                } else {
                    color_num(board[i][j].val);
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
    cout << endl << endl << RED << FLAG << RESET << ": " << mines - flags_used
         << endl;
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

/* check_board
 *    Purpose: A method to check if the game has ended, whether that be a win,
 *             a loss, or a continuation of the game.
 * Parameters: None
 *    Returns: A string representing the status of the game given the status
 *             of the board.
 */
string Game::check_board() {
    // If empty cell is revealed, make sure reveal all cells around it
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].rev and board[i][j].val == 0) {
                recurse_reveal(i, j);
            }
        }
    }
    // Check to see if the game is over
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].rev) {
                if (board[i][j].val == -1)  {
                    board[i][j].val = -2;
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

/* recurse_reveal
 *    Purpose: This function takes in Cell coordinates and if the Cell is empty
 *             on the board, then it reveals all the Cells around it, and then
 *             those Cells must replicate this process.
 * Parameters: Integers x and y, representing the coordinates of the Cell we
 *             are checking for emptiness.
 *    Returns: Nothing
 */
void Game::recurse_reveal(int x, int y) {
    if (board[x][y].val == 0) {
        if (board[x][y].empty_check) {
            return;
        }
        board[x][y].empty_check = true;
        if (x != 0) {
            if (y != 0) {
                board[x - 1][y - 1].rev = true;
                recurse_reveal(x - 1, y - 1);
            }
            board[x - 1][y].rev = true;
            recurse_reveal(x - 1, y);
            if (y != cols - 1) {
                board[x - 1][y + 1].rev = true;
                recurse_reveal(x - 1, y + 1);
            }
        }
        if (y != 0) {
            board[x][y - 1].rev = true;
            recurse_reveal(x, y - 1);
            if (x != rows - 1) {
                board[x + 1][y - 1].rev = true;
                recurse_reveal(x + 1, y - 1);
            }
        }
        if (x != rows - 1) {
            board[x + 1][y].rev = true;
            recurse_reveal(x + 1, y);
            if (y != cols - 1) {
                board[x + 1][y + 1].rev = true;
                recurse_reveal(x + 1, y + 1);
            }
        }
        if (y != cols - 1) {
            board[x][y + 1].rev = true;
            recurse_reveal(x, y + 1);
        }
    }
}

/* isValidInput
 *    Purpose: Input validation for coordinates in command loop.
 * Parameters: A string containing the user's inputted line, a character which
 *             will hold onto the 'f' in the command if it is inputted, and
 *             integers x and y, representing the coordinates of the Cell with
 *             which the user interacts.
 *    Returns: A boolean, which is true if the input is valid, false otherwise.
 */
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
