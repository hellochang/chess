#ifndef COMPUTER_H
#define COMPUTER_H

#include "GLOBALS.h"
#include "Player.h"

class Computer : public Player
{
    int level;

    void moveLv1to3(int level);

    void moveLv4();

public:
    Computer(Board &b, bool isWhite, int level = 1);

    virtual int move() override;

    virtual void setLevel(int level) override;
};

#endif
