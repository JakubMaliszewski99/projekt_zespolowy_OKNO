#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../include/Actor.h"
#define _USE_MATH_DEFINES
#include <cmath>


class Player: public Actor{

    public:
        Player(CollisionSolver* newSolver);
        ~Player();

    private:
        
};

#endif // PLAYER_H