#include "Human.h"

Human::Human(Board &b, bool isWhite) : Player(b, isWhite) {}

int Human::move()
{
    // init
    eval();
    oppo.lock()->eval();
    if (KIdx == -1)
        return 1;
    avoidKingKillingMove(); // throw away king killing moves
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
            b.cells[dest] = promo ? words[3][0] : b.cells[orig];
            b.cells[orig] = 0;
            return 100;
        }
    }
    return 0; // reach EOF
}

// cerr << "DEBUG: Got here!~" << endl;
// cerr << "Idx:" << endl;
// for (auto i : idx)
//     cerr << "    " << (int)i << endl;
// cerr << "moveIdx:" << endl;
// for (auto i : moveIdx)
// {
//     cerr << "    From: " << (int)toRow(i.first) << "," << (int)toCol(i.first) << endl;
//     for (auto j : i.second)
//         cerr << "        To: " << (int)toRow(j) << "," << (int)toCol(j) << endl;
// }
// cerr << "atkIdx:" << endl;
// for (auto i : atkIdx)
// {
//     cerr << "    From: " << (int)toRow(i.first) << "," << (int)toCol(i.first) << endl;
//     for (auto j : i.second)
//         cerr << "        To: " << (int)toRow(j) << "," << (int)toCol(j) << endl;
// }
// cerr << "DEBUG: Got here!~" << endl;