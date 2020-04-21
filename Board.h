#ifndef BOARD_H
#define BOARD_H

#include "GLOBALS.h"

class Board
{
public:
    vector<char> cells;
    bool isWhiteTurn = true;

    // Init to empty
    Board();

    // Init to std board. n's value doesn't matter here.
    Board(int n);

    // Gets piece by cells index
    char &piece(int idx);

    // Gets piece by row and column numbers
    char &piece(int row, int col);

    // Clears board (for setup)
    void clear();

    // Prints board (before move)
    void printBoard();

};

#endif
