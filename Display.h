#ifndef DISPLAY_H
#define DISPLAY_H

#include "GLOBALS.h"
#include "Board.h"

class Display
{
private:
    void printTextBoard(Board &b);
    void printGraphicBoard(Board &b);

public:
    void printBoard(Board &b);
};

#endif