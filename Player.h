#ifndef PLAYER_H
#define PLAYER_H

#include "GLOBALS.h"
#include "Board.h"

class Computer;
class Human;

class Player
{
    friend class Computer;
    friend class Human;

    Board &b;
    bool isWhite;
    const set<char> &myPieces;
    weak_ptr<Player> oppo;
    // index for King
    int KIdx = -1;
    // index for the pawn possible be captured by en passant
    int enPassantIdx = -1;
    // indicate if the player can do long castling or not, init by ctor
    bool canLongCastling;
    // indicate if the player can do short castling or not, init by ctor
    bool canShortCastling;
    // indices for my remaining pieces
    set<int> idx;
    // indices that are movable and corresponding dests
    map<int, set<int>> moveIdx;
    // indices that can attack (but not check) and corresponding dests
    map<int, set<int>> atkIdx;
    // indices that is checking opponent
    map<int, set<int>> checkIdx;
    // valid only if oppo.lock()->enPassantIdx >= 0,
    //  arranged as {origIdx, destIdx, capturedIdx}
    vector<int> enPassantMove;

    // Checks poses on the same row/col
    void checkSameRC(int origIdx, int row, int col);

    // Checks poses on the same diagonal
    void checkSameDiag(int origIdx, int row, int col);

    // Checks 8 possible poses for Knight
    void checkKnight(int origIdx, int row, int col);

    // Checks 8 possible poses for King
    void checkKing(int origIdx, int row, int col);

    // Verifys if a move is valid, if yes, insert it to corresponding set
    bool verify(int origIdx, int row, int col,
                bool checkMove = true, bool checkAtk = true);

    // Move out king-killing moves from three "moveable" sets
    void avoidKingKillingMove();

public:
    // Constructor
    Player(Board &b, bool isWhite);

    // Destructor
    virtual ~Player();

    // Make a move. For return values, 0 for EOF, 1 for lose,
    //  2 for draw, 3 for regret, others for continue
    virtual int move() = 0;

    // Initialize attributes of Player
    //  (before move() or board validity checking)
    bool eval();

    // Binds opponent ptr
    void bindOppo(shared_ptr<Player> other);

    // Returns true if checked
    bool inCheck();
};

#endif
