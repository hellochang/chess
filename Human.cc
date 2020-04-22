#include "Human.h"

// Human(b, isWhite) constructs a Human object
Human::Human(Board &b, bool isWhite) : Player(b, isWhite) {}

// move() makes a move and returns indicator values (0 for EOF,
//   1 for lose, 2 for draw, 3 for regret, others for continue)
int Human::move()
{
    // init
    eval();
    oppo.lock()->eval();
    if (KIdx == -1)
        return 1;

    // throw away king killing moves
    avoidKingKillingMove();

    // provide castling moves when possible
    int kRow = toRow(KIdx), kCol = toCol(KIdx);
    int kshortDest = toIdx(kRow, kCol + 2), kLongDest = toIdx(kRow, kCol - 3);
    // no castling under checking
    if ((canShortCastling || canLongCastling) && !inCheck())
    {
        if (canShortCastling && b.cells[toIdx(kRow, kCol + 1)] == 0 && b.cells[toIdx(kRow, kCol + 2)] == 0)
            moveIdx[KIdx].insert(kshortDest);
        if (canLongCastling && b.cells[toIdx(kRow, kCol - 1)] == 0 && b.cells[toIdx(kRow, kCol - 2)] == 0 && b.cells[toIdx(kRow, kCol - 3)] == 0)
            moveIdx[KIdx].insert(kLongDest);
    }

    vector<string> words;
    while (getWords(words))
    {
        int orig, dest;
        if (!words.size()) // empty command
            continue;
        if (moveIdx.size() + atkIdx.size() + checkIdx.size() + enPassantMove.size() == 0) // draw
        {
            return 2;
        }
        if (words.size() == 1 && words[0] == "regret") // regret
        {
            return 3;
        }
        else if (words.size() == 1 && words[0] == "resign") // resign
        {
            cout << "Resign!" << endl;
            return 1;
        }
        else if (words.size() != 3 && words.size() != 4) // bad input
        {
            badInput();
        }
        else if (!coordToIdx(words[1], orig) || !coordToIdx(words[2], dest)) // invalid coord
        {
            cout << "Error: Invalid positon!" << endl;
        }
        else if (!idx.count(orig)) // no piece or not my piece
        {
            cout << "Error: No such a piece!" << endl;
        }
        else if (!moveIdx[orig].count(dest) &&
                 !atkIdx[orig].count(dest) &&
                 !checkIdx[orig].count(dest) &&
                 !(enPassantMove.size() && enPassantMove[0] == orig && enPassantMove[1] == dest)) // cannot validly move it
        {
            cout << "Error: Invalid move!" << endl;
        }
        else
        {
            bool promo = false;
            if ((b.cells[orig] == 'P' && toRow(orig) == 7) ||
                (b.cells[orig] == 'p' && toRow(orig) == 2)) // pawn about to promote
            {
                if (words.size() == 4 && words[3][0] != b.cells[orig] &&
                    myPieces.count(words[3][0]))
                    promo = true;
                else
                {
                    cout << "Error: You should properly promote that pawn!" << endl;
                    continue;
                }
            }
            else if (words.size() == 4) // an unnecessary word in that command
            {
                badInput();
                continue;
            }
            // moves that piece, promote it when needed

            //  if this is a en passant move, capture that pawn
            if (enPassantMove.size() && enPassantMove[0] == orig && enPassantMove[1] == dest)
                b.cells[enPassantMove[2]] = 0;
            enPassantMove.clear();

            //  if this move gives opponent a chance to en passant, flag it
            if ((b.cells[orig] == 'P' && toRow(orig) == 2 && toRow(dest) == 4) ||
                (b.cells[orig] == 'p' && toRow(orig) == 7 && toRow(dest) == 5))
                enPassantIdx = dest;

            // if this is a castling move, move correspond rook
            if ((canShortCastling || canLongCastling) && orig == KIdx)
            {
                if (dest == kshortDest)
                {
                    b.cells[toIdx(kRow, kCol + 1)] = b.cells[toIdx(kRow, kCol + 3)];
                    b.cells[toIdx(kRow, kCol + 3)] = 0;
                }
                else if (dest == kLongDest)
                {
                    b.cells[toIdx(kRow, kCol - 2)] = b.cells[toIdx(kRow, kCol - 4)];
                    b.cells[toIdx(kRow, kCol - 4)] = 0;
                }
            }

            b.cells[dest] = promo ? words[3][0] : b.cells[orig];
            b.cells[orig] = 0;

            // if king/rooks is moved, disable corresponding castling
            if (orig == KIdx)
            {
                canLongCastling = false;
                canShortCastling = false;
            }
            if (isWhite)
            {
                if (b.cells[toIdx(1, 1)] != 'R')
                    canLongCastling = false;
                if ((b.cells[toIdx(1, 8)]) != 'R')
                    canShortCastling = false;
            }
            else
            {
                if (b.cells[toIdx(8, 1)] != 'r')
                    canLongCastling = false;
                if ((b.cells[toIdx(8, 8)]) != 'r')
                    canShortCastling = false;
            }

            return 100;
        }
    }
    return 0; // reach EOF
}

// Debugging Code
#if 0
cerr << "DEBUG: Got here!~" << endl;
cerr << "Idx:" << endl;
for (auto i : idx)
    cerr << "    " << (int)i << endl;
cerr << "moveIdx:" << endl;
for (auto i : moveIdx)
{
    cerr << "    From: " << (int)toRow(i.first) << "," << (int)toCol(i.first) << endl;
    for (auto j : i.second)
        cerr << "        To: " << (int)toRow(j) << "," << (int)toCol(j) << endl;
}
cerr << "atkIdx:" << endl;
for (auto i : atkIdx)
{
    cerr << "    From: " << (int)toRow(i.first) << "," << (int)toCol(i.first) << endl;
    for (auto j : i.second)
        cerr << "        To: " << (int)toRow(j) << "," << (int)toCol(j) << endl;
}
cerr << "DEBUG: Got here!~" << endl;
#endif
