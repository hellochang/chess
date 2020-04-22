#include "Board.h"
#include "window.h"

// Board() initialize to an empty Board
Board::Board() : cells{vector<char>(64, '\0')} {}

// Board(int n) Init to standard chess board.
Board::Board(int n) : cells{vector<char>{
                          'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                          'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
                          '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                          '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                          '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                          '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                          'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                          'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}} {}

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

// isBoardWhiteTurn(isCustomized) returns the value of the field isCustomized
bool Board::isBoardCustomized() {
    return isCustomized;
}

// setCustomized(isCustom) sets the field isCustomized to the given boolean
void Board::setCustomized(bool isCustom)
{
    isCustomized = isCustom;
}

