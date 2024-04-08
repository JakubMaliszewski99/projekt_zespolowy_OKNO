#include "../include/Actor.h"

// Konstruktor
Actor::Actor(){
    std::clog << "Utworzenie obiektu Actor" << std::endl;
}

Actor::Actor(CollisionSolver *newSolver): Object(newSolver)
{
} 

// Destruktor
Actor::~Actor(){}

// Ruch na minimapce
void Actor::move(sf::Vector2f movement){
    positionX += movement.x;
    positionY += movement.y;
}

// Rotacja
void Actor::rotate(float rotation){
    angle += rotation;
}

//Gettery

float Actor::getAngle(){
    return angle;
}

float Actor::getSpeed(){
    return speed;
}

float Actor::getRotationSpeed(){
    return rotationSpeed;
}

//Settery

void Actor::setSpeed(float speed){
    this->speed = speed;
}