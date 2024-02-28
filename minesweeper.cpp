/*
 *      minesweeper.cpp
 *      Minesweeper
 *      Project 2
 *      by Jack Tobia, 12/29/2023
 *
 *      The main function of the program, where the Game is created so that
 *      the user can play a custom game of Minesweeper! Can play multiple
 *      games, and will track wins and losses for when the user is done
 *      playing.
 *
 */

#include "Game.h"

using namespace std;

int main() {
    int games_won = 0, games_played = 0;
    Game game;
    games_won += game.play_game();
    games_played++;
    string play_again = "";
    bool keep_playing = true;
    while (keep_playing) {
        while (play_again != "y" and play_again != "n") {
            cout << "Would you like to play again? (y/n): ";
            cin >> play_again;
        }
        if (play_again == "y") {
            Game more_game;
            games_won += more_game.play_game();
            games_played++;
            play_again = "";
        } else {
            keep_playing = false;
        }
    }
    cout << "\nYou won " << games_won << " out of " << games_played
         << " games!\n";
    cout << "Hope you had fun, have a nice day!\n";
    return 0;
}
