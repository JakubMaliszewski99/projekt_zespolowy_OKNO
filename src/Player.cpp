#include "../include/Player.h"
// Konstruktor
Player::Player(){
    std::clog << "Utworzenie obiektu player" << std::endl;

    // Inicjalizacja zmiennych
    positionX = 0.0;
    positionY = 0.0;
    velocity = 0.0;

    isMovingForwards = isMovingBackwards = isMovingLeft = isMovingRight = false;
    isRotatingLeft = isRotatingRight = false;
    isSprinting = false;

    speed = PLAYER_DEFAULT_SPEED;
    rotationSpeed = M_PI/180.f * 500;
}

// Destruktor
Player::~Player(){}

// Ruch na minimapce
void Player::move(sf::Vector2f movement){
    positionX += movement.x;
    positionY += movement.y;}

// Rotacja
void Player::rotate(float rotation){
    angle += rotation;
}

//Gettery

double Player::getPositionX(){
    return positionX;
}
double Player::getPositionY(){
    return positionY;
}

float Player::getAngle(){
    return angle;
}

float Player::getSpeed(){
    return speed;
}

float Player::getRotationSpeed(){
    return rotationSpeed;
}

//Settery

void Player::setSpeed(float speed){
    this->speed = speed;
}