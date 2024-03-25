#ifndef ENEMY_H
#define ENEMY_H

#pragma once
#include "../include/Actor.h"

class Enemy: public Actor{
     
public:
    Enemy();
    Enemy(int windowSizeX, int windowSizeY);
    ~Enemy();

private:

};

#endif