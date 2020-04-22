#include "Display.h"

void Display::printTextBoard(Board &b)
{
    cout << endl
         << "Current board is:" << endl;
    for (int row = 8; row >= 1; --row)
    {
        cout << row << " ";
        for (int col = 1; col <= 8; ++col)
        {
            char pc = b.piece(row, col);
            char BnW = (row + col) % 2 ? ' ' : '_';
            cout << (pc ? pc : BnW);
        }
        cout << endl;
    }
    cout << endl
         << "  abcdefgh" << endl;
}

void Display::printBoard(Board &b)
{
    printTextBoard(b);
    if (isGraphic)
        printGraphicBoard(b);
}

Display::Display(bool isGraphic) : isGraphic{isGraphic} {}

void Display::printGraphicBoard(Board &b)
{
    Xwindow w;
    w.fillRectangle(0, 0, 500, 500, Xwindow::Blue);
    w.drawString(50, 50, "Current board is:", Xwindow::Blue);
    // todo
}