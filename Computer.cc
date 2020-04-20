#include "Computer.h"

Computer::Computer(Board &b, bool isWhite, int level)
    : Player(b, isWhite), level{level} {}

int Computer::move()
{
    eval();
    oppo.lock()->eval();
    if (KIdx == -1)
        return 1;
    avoidKingKillingMove(); // throw away king killing moves

    vector<string> words;
    while (getWords(words))
    {
        if (!words.size())
            continue;
        if (words.size() == 1 && words[0] == "move")
        {
            if (moveIdx.size() + atkIdx.size() + checkIdx.size() + enPassantMove.size())
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
        else
            badInput();
    }
    return 0; // reach EOF
}

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
        randomAtkSet.push_back(pair<int, int>{enPassantMove[0], enPassantMove[1]});
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
        if (randomAtkSet.size()) // if possible, prefer attacks over other moves
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
        //  if this is a en passant move, capture that pawn first
        if (enPassantMove.size() && enPassantMove[0] == orig && enPassantMove[1] == dest)
            b.cells[enPassantMove[3]] = 0;
        b.cells[dest] = b.cells[orig];
        b.cells[orig] = 0;
    }
}

void Computer::moveLv4()
{
    moveLv1to3(3);
}