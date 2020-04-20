#include "Board.h"

Board::Board() : cells{vector<char>(64, '\0')} {}

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

char &Board::piece(int idx)
{
    return cells[idx];
}

char &Board::piece(int row, int col)
{
    return cells[toIdx(row, col)];
}
void Board::clear()
{
    isWhiteTurn = true;
    cells = vector<char>(64, '\0');
}

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