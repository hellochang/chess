#include "Render.h"

// printTextBoard(b) displays the Board in text
void Render::printTextBoard(Board &b)
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


// printGraphicBoard(b) displays the Board in graphics
void Render::printGraphicBoard(Board &b)
{
    w.fillRectangle(0, 0, 510, 510, Xwindow::White);
    w.drawString(10, 10, "Current board is:", Xwindow::Black);

    const int STRING_SIZE{35};
    const int CELL_SIZE{50};
    int x{STRING_SIZE};
    for (int row{8}; row >= 1; --row)
    {
        // drawing the initial vertical numbered coordinates
        w.drawBigString(5, x + STRING_SIZE + 5, to_string(row) + "  ", Xwindow::Black);

        int y{5 + STRING_SIZE};
        for (int col = 1; col <= 8; ++col)
        {
            char pc = b.piece(row, col);
            bool isCellWhite{(row + col) % 2 ? true : false};
            w.fillRectangle(y, x, CELL_SIZE, CELL_SIZE, isCellWhite ? Xwindow::White : Xwindow::Black);
            if (pc)
            {
                string s(1, pc);
                w.drawBigString(y + 15, x + STRING_SIZE, s, Xwindow::Yellow);
            }
            y += CELL_SIZE + 1;
        }
        x += CELL_SIZE + 1;
        w.drawBigString(STRING_SIZE, y + STRING_SIZE,
                        " a   b   c   d   e    f   g   h", Xwindow::Black);
    }
}

// printBoard(b) displays the Board in graphic
void Render::printBoard(Board &b)
{
    isGraphic ? printGraphicBoard(b) : printTextBoard(b);
    
}

// Render(isGraphic) constructs a Render object
Render::Render(bool isGraphic) : isGraphic{isGraphic} {}
