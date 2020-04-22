#ifndef HUMAN_H
#define HUMAN_H

#include "GLOBALS.h"
#include "Player.h"

class Human : public Player
{
public:
    Human(Board &b, bool isWhite);

    virtual int move() override;
};

#endif
