#include "Player.h"

// Player(b, isWhite) constructs the Player object
Player::Player(Board &b, bool isWhite)
    : b{b}, isWhite{isWhite},
      myPieces{isWhite ? WHITEPIECESET : BLACKPIECESET},
      canLongCastling{!b.checkCustomized()}, canShortCastling{!b.checkCustomized()} {}

// Player() destroys the Player object
Player::~Player() {}

// checkSameRC(origIdx, row, col) checks poses on the same row/col
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

// checkSameDiag(origIdx, row, col) checks poses on the same diagonal
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

// checkKnight(int origIdx, int row, int col) checks 8 possible
//  poses for Knight
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

// checkKing(origIdx, row, col) checks 8 possible poses for King
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

// verify(origIdx, row, col, checkMove, checkAtk) verifys if a move
//  is valid, if yes, insert it to corresponding set
bool Player::verify(int origIdx, int row,
                    int col, bool checkMove, bool checkAtk)
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
            if (b.piece(newIdx) == 'K' || b.piece(newIdx) == 'k')
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
        if (b.piece(newIdx) == 0)
            moveIdx[origIdx].insert(newIdx);
        else
            cont = false;
    }
    return cont;
}

// avoidKingKillingMove() invoked to avoid moves that puts King in check
void Player::avoidKingKillingMove()
{
    set<int> underAtk; // poses under attack
    for (auto i : oppo.lock()->atkIdx)
        for (auto j : i.second)
            if (!underAtk.count(j))
                underAtk.insert(j);
    if (moveIdx.count(KIdx))
        for (auto i : underAtk)
            moveIdx[KIdx].erase(i);
    if (atkIdx.count(KIdx))
        for (auto i : underAtk)
            atkIdx[KIdx].erase(i);
    if (checkIdx.count(KIdx))
        for (auto i : underAtk)
            checkIdx[KIdx].erase(i);
}

// eval() initialize attributes of Player
//  (before move() or board validity checking)
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
        if (myPieces.count(b.piece(i)))
            idx.insert(i);
        else if (oppo.lock()->myPieces.count(b.piece(i)))
            oppo.lock()->idx.insert(i);
    for (auto i : idx)
    {
        int row = toRow(i);
        int col = toCol(i);
        if (b.piece(i) == 'R' || b.piece(i) == 'r')
        {
            checkSameRC(i, row, col);
        }
        else if (b.piece(i) == 'N' || b.piece(i) == 'n')
        {
            checkKnight(i, row, col);
        }
        else if (b.piece(i) == 'B' || b.piece(i) == 'b')
        {
            checkSameDiag(i, row, col);
        }
        else if (b.piece(i) == 'Q' || b.piece(i) == 'q')
        {
            checkSameRC(i, row, col);
            checkSameDiag(i, row, col);
        }
        else if (b.piece(i) == 'K' || b.piece(i) == 'k')
        {
            checkKing(i, row, col);
            KIdx = i;
            ++kingCount;
        }
        else if (b.piece(i) == 'P') // Pawns are more complicated
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
                    enPassantMove = vector<int>{i,
                                                toIdx(row + 1, col - 1),
                                                left};
                    verify(i, row + 1, col + 1, false, true);
                }
                else if (oppo.lock()->enPassantIdx == right)
                {
                    verify(i, row + 1, col, true, false);
                    verify(i, row + 1, col - 1, false, true);
                    enPassantMove = vector<int>{i,
                                                toIdx(row + 1, col + 1),
                                                right};
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
        else if (b.piece(i) == 'p') // Pawns are more complicated
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
                    enPassantMove = vector<int>{i,
                                                toIdx(row - 1, col - 1), left};
                    verify(i, row - 1, col + 1, false, true);
                }
                else if (oppo.lock()->enPassantIdx == right)
                {
                    verify(i, row - 1, col, true, false);
                    verify(i, row - 1, col - 1, false, true);
                    enPassantMove = vector<int>{i,
                                                toIdx(row - 1, col + 1),
                                                right};
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

// bindOppo (other) binds opponent ptr
void Player::bindOppo(shared_ptr<Player> other)
{
    oppo = other;
}

// inCheck() returns true if checked
bool Player::inCheck()
{
    return oppo.lock()->checkIdx.size() > 0;
}
