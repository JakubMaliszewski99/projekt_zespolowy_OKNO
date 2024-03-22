#ifndef WEAPON_H
#define WEAPON_H

#include "../include/Player.h"

class Weapon {
public:
    Weapon();
    ~Weapon();

    virtual void fire() = 0;

private:
    Player* owner;
};

#endif // WEAPON_H