#include "Player.h"

// Player(b, isWhite) constructs the Player object
Player::Player(Board &b, bool isWhite)
    : b{b}, myPieces{isWhite ? WHITEPIECESET : BLACKPIECESET} {}


// Player() destroys the Player object
Player::~Player() {}


// 
void Player::checkSameRC(int origIdx, int row, int col)
{
    for (int j = col + 1;; ++j)
        if (!verify(origIdx, row, j))
            break;
    for (int j = col - 1;; --j)
        if (!verify(origIdx, row, j))
            break;
    for (int j = row + 1;; ++j)
        if (!verify(origIdx, j, col))
            break;
    for (int j = row - 1;; --j)
        if (!verify(origIdx, j, col))
            break;
}

void Player::checkSameDiag(int origIdx, int row, int col)
{
    for (int j = 1;; ++j)
        if (!verify(origIdx, row + j, col + j))
            break;
    for (int j = 1;; ++j)
        if (!verify(origIdx, row - j, col + j))
            break;
    for (int j = 1;; ++j)
        if (!verify(origIdx, row + j, col - j))
            break;
    for (int j = 1;; ++j)
        if (!verify(origIdx, row - j, col - j))
            break;
}

void Player::checkKnight(int origIdx, int row, int col)
{
    verify(origIdx, row + 1, col + 2);
    verify(origIdx, row + 1, col - 2);
    verify(origIdx, row - 1, col + 2);
    verify(origIdx, row - 1, col - 2);
    verify(origIdx, row + 2, col + 1);
    verify(origIdx, row + 2, col - 1);
    verify(origIdx, row - 2, col + 1);
    verify(origIdx, row - 2, col - 1);
}

void Player::checkKing(int origIdx, int row, int col)
{
    verify(origIdx, row - 1, col - 1);
    verify(origIdx, row - 1, col);
    verify(origIdx, row - 1, col + 1);
    verify(origIdx, row, col - 1);
    verify(origIdx, row, col + 1);
    verify(origIdx, row + 1, col - 1);
    verify(origIdx, row + 1, col);
    verify(origIdx, row + 1, col + 1);
}

bool Player::verify(int origIdx, int row, int col, bool checkMove, bool checkAtk)
{
    if (!(1 <= row && row <= 8) || !(1 <= col && col <= 8))
        return false;
    bool cont = true; // should continue verifying forward or not
    int newIdx = toIdx(row, col);
    if (idx.count(newIdx))
        return false;
    if (checkAtk)
    {
        if (oppo.lock()->idx.count(newIdx))
        {
            if (b.cells[newIdx] == 'K' || b.cells[newIdx] == 'k')
            {
                checkIdx[origIdx].insert(newIdx);
            }
            else
                atkIdx[origIdx].insert(newIdx);
            cont = false;
        }
    }
    if (checkMove)
    {
        if (b.cells[newIdx] == 0)
            moveIdx[origIdx].insert(newIdx);
        else
            cont = false;
    }
    return cont;
}

bool Player::eval()
{
    KIdx = -1;
    enPassantIdx = -1;
    enPassantMove.clear();
    idx.clear();
    oppo.lock()->idx.clear(); // clear oppo idx for verifying
    moveIdx.clear();
    atkIdx.clear();
    checkIdx.clear();

    int kingCount = 0;
    for (int i = 0; i < 64; ++i)
        if (myPieces.count(b.cells[i]))
            idx.insert(i);
        else if (oppo.lock()->myPieces.count(b.cells[i]))
            oppo.lock()->idx.insert(i);
    for (auto i : idx)
    {
        int row = toRow(i);
        int col = toCol(i);
        if (b.cells[i] == 'R' || b.cells[i] == 'r')
        {
            checkSameRC(i, row, col);
        }
        else if (b.cells[i] == 'N' || b.cells[i] == 'n')
        {
            checkKnight(i, row, col);
        }
        else if (b.cells[i] == 'B' || b.cells[i] == 'b')
        {
            checkSameDiag(i, row, col);
        }
        else if (b.cells[i] == 'Q' || b.cells[i] == 'q')
        {
            checkSameRC(i, row, col);
            checkSameDiag(i, row, col);
        }
        else if (b.cells[i] == 'K' || b.cells[i] == 'k')
        {
            checkKing(i, row, col);
            KIdx = i;
            ++kingCount;
        }
        else if (b.cells[i] == 'P') // Pawns are more complicated
        {
            if (row == 1 || row == 8)
                return false;
            if (row == 2)
            {
                if (verify(i, row + 1, col, true, false))
                    verify(i, row + 2, col, true, false);
                verify(i, row + 1, col - 1, false, true);
                verify(i, row + 1, col + 1, false, true);
            }
            else if (row == 5)
            {
                int left = toIdx(row, col - 1);
                int right = toIdx(row, col + 1);
                if (oppo.lock()->enPassantIdx == left)
                {
                    verify(i, row + 1, col, true, false);
                    enPassantMove = vector<int>{i, toIdx(row + 1, col - 1), left};
                    verify(i, row + 1, col + 1, false, true);
                }
                else if (oppo.lock()->enPassantIdx == right)
                {
                    verify(i, row + 1, col, true, false);
                    verify(i, row + 1, col - 1, false, true);
                    enPassantMove = vector<int>{i, toIdx(row + 1, col + 1), right};
                }
                else
                {
                    verify(i, row + 1, col, true, false);
                    verify(i, row + 1, col - 1, false, true);
                    verify(i, row + 1, col + 1, false, true);
                }
            }
            else
            {
                verify(i, row + 1, col, true, false);
                verify(i, row + 1, col - 1, false, true);
                verify(i, row + 1, col + 1, false, true);
            }
        }
        else if (b.cells[i] == 'p') // Pawns are more complicated
        {
            if (row == 1 || row == 8)
                return false;
            if (row == 7)
            {
                if (verify(i, row - 1, col, true, false))
                    verify(i, row - 2, col, true, false);
                verify(i, row - 1, col - 1, false, true);
                verify(i, row - 1, col + 1, false, true);
            }
            else if (row == 5)
            {
                int left = toIdx(row, col - 1);
                int right = toIdx(row, col + 1);
                if (oppo.lock()->enPassantIdx == left)
                {
                    verify(i, row - 1, col, true, false);
                    enPassantMove = vector<int>{i, toIdx(row - 1, col - 1), left};
                    verify(i, row - 1, col + 1, false, true);
                }
                else if (oppo.lock()->enPassantIdx == right)
                {
                    verify(i, row - 1, col, true, false);
                    verify(i, row - 1, col - 1, false, true);
                    enPassantMove = vector<int>{i, toIdx(row - 1, col + 1), right};
                }
                else
                {
                    verify(i, row - 1, col, true, false);
                    verify(i, row - 1, col - 1, false, true);
                    verify(i, row - 1, col + 1, false, true);
                }
            }
            else
            {
                verify(i, row - 1, col, true, false);
                verify(i, row - 1, col - 1, false, true);
                verify(i, row - 1, col + 1, false, true);
            }
        }
    }
    return kingCount == 1;
}

void Player::bindOppo(shared_ptr<Player> other)
{
    oppo = other;
}

// inCheck() returns true if checked
bool Player::inCheck()
{
    return oppo.lock()->checkIdx.size() > 0;
}
