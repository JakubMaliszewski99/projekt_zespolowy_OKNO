#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/Object.h"

//sta�e
const float DEFAULT_SPEED = 100.f;
const float PLAYER_DEFAULT_SPEED = 150.f; //pocz�tkowa pr�dko�� gracza
const float ENEMY_DEFAULT_SPEED = 70.f;
const float DEFAULT_ROTATION_SPEED = 500;

class Actor: public Object{

    public:
        Actor();
        ~Actor();

        // Ruch
        void move(sf::Vector2f movement);

        // Rotacja
        void rotate(float rotation);

        // Gettery
        double getPositionX();
        double getPositionY();
        float getAngle();
        float getSpeed();
        float getRotationSpeed();

        // Settery
        void setSpeed(float speed);

        // Zmienne stanu gracza
        bool isMovingForwards, isMovingBackwards, isMovingLeft, isMovingRight;
        bool isRotatingLeft, isRotatingRight;
        bool isSprinting;

    protected:
        // Zmienne do ruchu gracza
        double positionX;
        double positionY;
        double velocity;
        float angle; // w radianach

        // Zmienne do logiki gry
        float life;
        float speed;
        float rotationSpeed;
        
};

#endif // ACTOR_H