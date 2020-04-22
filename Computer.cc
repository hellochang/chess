#include "Computer.h"

// Computer(b, isWhite, level) constructs a Computer object
Computer::Computer(Board &b, bool isWhite, int level)
    : Player(b, isWhite), level{level} {}

// move() makes a move and returns indicator values (0 for EOF, 1 for lose,
//   2 for draw, 3 for regret, others for continue)
int Computer::move()
{
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
        if (canShortCastling && b.cells[toIdx(kRow, kCol + 1)] == 0 
        && b.cells[toIdx(kRow, kCol + 2)] == 0)
            moveIdx[KIdx].insert(kshortDest);
        if (canLongCastling && b.cells[toIdx(kRow, kCol - 1)] == 0 
        && b.cells[toIdx(kRow, kCol - 2)] == 0 
        && b.cells[toIdx(kRow, kCol - 3)] == 0)
            moveIdx[KIdx].insert(kLongDest);
    }

    vector<string> words;
    while (getWords(words))
    {
        if (!words.size())
            continue;
        if (words.size() == 1 && words[0] == "move")
        {
            if (moveIdx.size() + atkIdx.size() + checkIdx.size() 
            + enPassantMove.size())
            {
                switch (level)
                {
                case 1:
                case 2:
                case 3:
                    moveLv1to3(level);
                    return 100;
                default:
                    moveLv4();
                    return 100;
                }
            }
            else
            {
                return 2; // draw
            }
        }
        else if (words.size() == 2 && words[0] == "setlevel" && words[1].length == 1)
        {
            setLevel(words[1][0] - '0');
            return move();
        }
        else
            badInput();
    }
    return 0; // reach EOF
}

// moveLv1to3(level) makes a move for Level 1 to 3
void Computer::moveLv1to3(int level)
{
    int orig, dest; // the move to be done

    vector<pair<int, int>> randomAtkSet; // all possible attack moves
    for (auto i : atkIdx)
        for (auto j : i.second)
            randomAtkSet.push_back(pair<int, int>{i.first, j});
    for (auto i : checkIdx)
        for (auto j : i.second)
            randomAtkSet.push_back(pair<int, int>{i.first, j});
    if (enPassantMove.size()) // add en passant move if possible
        randomAtkSet.push_back(pair<int, int>
        {enPassantMove[0], enPassantMove[1]});
    vector<pair<int, int>> randomSet = randomAtkSet; // all possible moves
    for (auto i : moveIdx)
        for (auto j : i.second)
            randomSet.push_back(pair<int, int>{i.first, j});

    set<int> piecesUnderAtk;
    switch (level)
    {
    case 3:
        for (auto i : oppo.lock()->atkIdx)
            for (auto j : i.second)
                if (!piecesUnderAtk.count(j))
                    piecesUnderAtk.insert(j);
        for (auto i : oppo.lock()->checkIdx)
            for (auto j : i.second)
                if (!piecesUnderAtk.count(j))
                    piecesUnderAtk.insert(j);
        if (piecesUnderAtk.size())
        {
            for (auto i : piecesUnderAtk)
                if (checkIdx.count(i))
                {
                    orig = i;
                    dest = *checkIdx[i].begin();
                    break;
                }
                else if (atkIdx.count(i))
                {
                    orig = i;
                    dest = *atkIdx[i].begin();
                    break;
                }
                else if (moveIdx.count(i))
                {
                    orig = i;
                    dest = *moveIdx[i].begin();
                    break;
                }
            // nothing to avoid, drop to Lv2
        }
        // nothing to avoid, drop to Lv2
    case 2:
    // if possible, prefer attacks over other moves
        if (randomAtkSet.size()) 
            randomSet = randomAtkSet;
    case 1:
    default:
        srand(time(nullptr));
        int choice = rand() % randomSet.size(); // choose a move
        orig = randomSet[choice].first;
        dest = randomSet[choice].second;
        break;
    }
    bool promo = (b.cells[orig] == 'P' && toRow(orig) == 7) ||
                 (b.cells[orig] == 'p' && toRow(orig) == 2);
    if (promo)
    {
        vector<char> promoSel;
        for (auto i : myPieces)
            if (i != 'k' && i != 'K')
                promoSel.push_back(i);
        char promotedTo = promoSel[rand() % promoSel.size()];
        b.cells[dest] = promotedTo;
        b.cells[orig] = 0;
    }
    else
    {
        //  if this is a en passant move, capture that pawn
        if (enPassantMove.size() && enPassantMove[0] == orig 
        && enPassantMove[1] == dest)
            b.cells[enPassantMove[2]] = 0;
        enPassantMove.clear();

        //  if this move gives opponent a chance to en passant, flag it
        if ((b.cells[orig] == 'P' && toRow(orig) == 2 && toRow(dest) == 4) ||
            (b.cells[orig] == 'p' && toRow(orig) == 7 && toRow(dest) == 5))
            enPassantIdx = dest;

        // if this is a castling move, move correspond rook
        int kRow = toRow(KIdx), kCol = toCol(KIdx);
        int kshortDest = toIdx(kRow, kCol + 2),
         kLongDest = toIdx(kRow, kCol - 3);
        if ((canShortCastling || canLongCastling) && orig == KIdx)
        {
            if (dest == kshortDest)
            {
                b.cells[toIdx(kRow, kCol + 1)] = 
                b.cells[toIdx(kRow, kCol + 3)];
                b.cells[toIdx(kRow, kCol + 3)] = 0;
            }
            else if (dest == kLongDest)
            {
                b.cells[toIdx(kRow, kCol - 2)] = 
                b.cells[toIdx(kRow, kCol - 4)];
                b.cells[toIdx(kRow, kCol - 4)] = 0;
            }
        }

        b.cells[dest] = b.cells[orig];
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
    }
}

// moveLv4(level) makes a move for Level 4
void Computer::moveLv4()
{
    moveLv1to3(3);
}

void Computer::setLevel(int level)
{
    if (level < 1 || level > 4)
        cout << "Level setting failed. Out of range! (range: 1-4)" << endl;
    else
        level = level;
}