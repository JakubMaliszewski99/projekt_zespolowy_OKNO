#include "../include/Object.h"

Object::Object()
{

}

Object::Object(CollisionSolver *newSolver)
{
    collisionSolver = newSolver;
}

Object::~Object()
{

}

double Object::getPositionX(){
    return positionX;
}
double Object::getPositionY(){
    return positionY;
}
