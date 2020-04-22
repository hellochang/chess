#ifndef DISPLAY_H
#define DISPLAY_H

#include "GLOBALS.h"
#include "Board.h"
#include "window.h"

class Display
{
private:
    bool isGraphic; // printGraphicBoard or not
    
    // Prints Board b as text
    void printTextBoard(Board &b);

    // Prints Board b in a window
    void printGraphicBoard(Board &b);

public:
    Display(bool isGraphic);

    // Prints Board b
    void printBoard(Board &b);
};

#endif