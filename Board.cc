#include "Board.h"

// Board() initialize to an empty Board
Board::Board() : cells{vector<char>(64, '\0')} {}


// Board(int n) Init to standard chess board.
Board::Board(int n) : cells{vector<char>{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                                         'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
                                         '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                         '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                         '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                         '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                         'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                                         'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}}
{
}


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


// printBoard() prints the entire chess board
void Board::printBoard()
{
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
