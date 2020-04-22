#include "Board.h"
#include "window.h"

// Board() initialize to an empty Board
Board::Board() : cells{vector<char>(64, '\0')} {}

// Board(int n) Init to standard chess board.
Board::Board(int n) : cells{vector<char> {
    'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
    'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
    'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
    }} {}

// piece(idx) gets piece by cells index
char &Board::piece(int idx)
{
    return cells[idx];
}

// piece(row, col) gets piece by row and column numbers
char &Board::piece(int row, int col)
{
    return cells[toIdx(row, col)];
}

// clear() clears board (for setup)
void Board::clear()
{
    isWhiteTurn = true;
    cells = vector<char>(64, '\0');
}

// setWhiteTurn(isWhite) sets the field isWhiteTurn
void Board::setWhiteTurn(bool isWhite)
{
    isWhiteTurn = isWhite;
}

// isBoardWhiteTurn(isWhite) returns the value of the field isWhiteTurn
bool Board::isBoardWhiteTurn()
{
    return isWhiteTurn;
}

// Sets the graphics field to true or false
void Board::setGraphic(bool graphic)
{
    isGraphic = graphic;
}

// setCustomized(isCustom) sets the field isCustomized to the given boolean
void setCustomized(bool isCustom)
{
    isCustomized = isCustom;
}

// printBoard() prints the entire chess board
void Board::printBoard()
{
    if (isGraphic)
    {
        printGraphicBoard();
        return;
    }
    cout << endl
         << "Current board is:" << endl;
    for (int row = 8; row >= 1; --row)
    {
        cout << row << " ";
        for (int col = 1; col <= 8; ++col)
        {
            char pc = piece(row, col);
            char BnW = (row + col) % 2 ? ' ' : '_';
            cout << (pc ? pc : BnW);
        }
        cout << endl;
    }
    cout << endl
         << "  abcdefgh" << endl;
}

// printGraphicBoard() prints a Board graphically
void Board::printGraphicBoard()
{
    Xwindow w;
    w.fillRectangle(0, 0, 510, 510, Xwindow::White);
    w.drawString(10, 10, "Current board is:", Xwindow::Black);

    const int STRING_SIZE{35};
    const int CELL_SIZE{50};
    int x{STRING_SIZE};
    for (int row = 8; row >= 1; --row)
    {
        // drawing the initial vertical numbered coordinates
        w.drawBigString(5, x + STRING_SIZE + 5, to_string(row) + "  ", Xwindow::Black);

        int y{5 + STRING_SIZE};
        for (int col = 1; col <= 8; ++col)
        {
            char pc = piece(row, col);
            bool isCellWhite{(row + col) % 2 ? true : false};
            w.fillRectangle(x, y, CELL_SIZE, CELL_SIZE, isCellWhite ? Xwindow::White : Xwindow::Black);
            if (pc)
            {
                string s(1, pc);
                w.drawBigString(x + 15, y + STRING_SIZE, s, Xwindow::Yellow);
            }
            y += CELL_SIZE + 1;
        }
        x += CELL_SIZE + 1;
        w.drawBigString(STRING_SIZE, y + STRING_SIZE,
                        " a   b   c   d   e    f   g   h", Xwindow::Black);
    }
}
