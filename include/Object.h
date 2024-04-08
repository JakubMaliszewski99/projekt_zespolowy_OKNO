#ifndef OBJECT_H
#define OBJECT_H

#pragma once

#include "../include/CollisionSolver.h"

class Object {
public:
    Object();
    Object(CollisionSolver* newSolver);
    ~Object();

    double getPositionX();
    double getPositionY();
    
protected:
    double positionX;
    double positionY;

    CollisionSolver* collisionSolver;
};

#endif