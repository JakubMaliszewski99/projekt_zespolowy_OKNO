#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>

//stałe
const double PLAYER_ROT = 3.141592653589/180.f * 5;

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
        float getRadius();
        sf::CircleShape getPlayerDot();
        sf::VertexArray getDirectionLine();
        float getSpeed();
        float getRotationSpeed();

        // Zmienne stanu gracza
        bool isMovingUp, isMovingDown, isMovingLeft, isMovingRight;
        bool isRotatingLeft, isRotatingRight;

    private:
        // Zmienne do ruchu gracza
        double positionX;
        double positionY;
        double velocity;
        float angle;

        // Rysowanie kropki gracza na minimapce
        float radius;
        sf::Color playerColor;
        sf::CircleShape playerDot;

        // Rysowanie linii gracza wyznaczającej zwrot gracza
        sf::Vector2f direction;
        sf::VertexArray directionLine;

        // Zmienne do logiki gry
        float life;
        float speed;
        float rotationSpeed;
        
};

#endif // PLAYER_H