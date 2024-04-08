#ifndef COLLISIONSOLVER_H
#define COLLISIONSOLVER_H

#pragma once

#include "../include/Object.h"
#include <list>

const int MIN_COLLISION_DISTANCE = 5;

class CollisionSolver
{
public:
    CollisionSolver();
    ~CollisionSolver();

    void addObject(Object* newObject);
    void removeObject(Object* removedObject);

    Object* collisionCheck(Object* objectToCheck);

private:
    std::list<Object*> objects;
    double distanceBetweenObjects(Object* first, Object* second);

};

#endif