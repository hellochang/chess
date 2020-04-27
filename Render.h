#ifndef RENDER_H
#define RENDER_H

#include "window.h"
#include "GLOBALS.h"
#include "Board.h"

// Render class renders the chessboard
class Render
{
    unique_ptr<Xwindow> wd;
    bool isGraphic;
    void printTextBoard(Board &b);
    void printGraphicBoard(Board &b);

public:
    void printBoard(Board &b);
    Render(bool isGraphic);
};

#endif

