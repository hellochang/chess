#ifndef GAME_H
#define GAME_H

#include "GLOBALS.h"
#include "Human.h"
#include "Computer.h"
#include "Board.h"
#include "Display.h"

class Game
{
    // W/B Score = 2 * real score
    // implemented to avoid accumulating float type precision problem
    int WScore = 0;
    int BScore = 0;
    Board b{1};
    vector<Board> history;
    shared_ptr<Player> p1;
    shared_ptr<Player> p2;
    Display display;
    bool isGraphic{false};

    // Runs game on current board
    bool run();

    // Sets up board
    bool setup();

    // Prints W/B real scores
    void printScores();

    // Checks validity of board
    bool validBoard();

    // Prints menu
    void printMenu();

    // Saves current Board to history field
    void saveBoard();

public:
    // Main logic goes here and commands are interpreted
    void startGame();

    // Set to true if the user choose graphic display
    void setGraphic(bool graphic);
};

#endif
