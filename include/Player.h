#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>


//stałe
const float PLAYER_DEFAULT_SPEED = 150.f; //początkowa prędkość gracza

class Player{

    public:
        Player();
        ~Player();

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

    private:
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

#endif // PLAYER_H