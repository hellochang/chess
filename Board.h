#ifndef BOARD_H
#define BOARD_H

#include "GLOBALS.h"

class Board
{
    vector<char> cells;
    bool isWhiteTurn{true};
    bool isCustomized{false};
    bool isGraphic{false};

public:  
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
    
    // Sets the graphics field to true or false
    void setGraphic(bool graphic);

    // Sets the isWhiteTurn field to true or false
    void setWhiteTurn(bool isWhite);

    // Sets the isCustomized field to true or false
    void setCustomized(bool isCustom);
    
    // Gets isWhiteTurn field 
    bool isBoardWhiteTurn(); 

    // Prints board graphically (before move)
    void printGraphicBoard();
    
    // // Prints board (before move)
    // void printBoard();
   
};

#endif
