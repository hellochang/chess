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

    int KIdx = -1;               // index for King
    int enPassantIdx = -1;       // index for the pawn possible be captured by en passant
    bool canLongCastling;        // indicate if the player can do long castling or not, init by ctor
    bool canShortCastling;       // indicate if the player can do short castling or not, init by ctor
    set<int> idx;                // indices for my remaining pieces
    map<int, set<int>> moveIdx;  // indices that are movable and corresponding dests
    map<int, set<int>> atkIdx;   // indices that can attack (but not check) and corresponding dests
    map<int, set<int>> checkIdx; // indices that is checking opponent
    vector<int> enPassantMove;   // valid only if oppo.lock()->enPassantIdx >= 0, arranged as {origIdx, destIdx, capturedIdx}

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

    // move out king-killing moves from three "moveable" sets
    void avoidKingKillingMove();

public:
    Player(Board &b, bool isWhite);

    virtual ~Player();
    
    //0 for EOF, 1 for lose, 2 for draw, 3 for regret, others for continue
    virtual int move() = 0;

    // Initialize attributes of Player (before move() or board validity checking)
    bool eval();

    // Binds opponent ptr
    void bindOppo(shared_ptr<Player> other);

    // Returns true if checked
    bool inCheck();
};

#endif
