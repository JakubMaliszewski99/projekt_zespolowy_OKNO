#include "../include/CollisionSolver.h"
#include <cmath>

CollisionSolver::CollisionSolver()
{

}

CollisionSolver::~CollisionSolver()
{

}

void CollisionSolver::addObject(Object* newObject)
{
    objects.push_back(newObject);
}

void CollisionSolver::removeObject(Object* removedObject)
{
    objects.remove(removedObject);
}

Object* CollisionSolver::collisionCheck(Object* objectToCheck)
{
    for(auto o: objects)
    {
        if(o == objectToCheck)
            continue;
        if( distanceBetweenObjects(objectToCheck, o) < MIN_COLLISION_DISTANCE)
            return o;
    }
    return nullptr;
}

double CollisionSolver::distanceBetweenObjects(Object* first, Object* second)
{
    int xDistance = std::abs(first->getPositionX() - second->getPositionX());
    int yDistance = std::abs(first->getPositionY() - second->getPositionY());

    return std::sqrt((xDistance^2) + (yDistance^2));
}