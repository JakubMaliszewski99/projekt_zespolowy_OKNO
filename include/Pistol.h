#ifndef PISTOL_H
#define PISTOL_H

#include "../include/Weapon.h"

class Pistol: public Weapon {
public:
    Pistol();
    ~Pistol();

    virtual void fire();

private:
    
};

#endif // PISTOL_H