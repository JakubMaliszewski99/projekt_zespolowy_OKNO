#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>

//stałe
const double PLAYER_MOV = 2.5;
const double PLAYER_ROT = 3.141592653589/180.f * 5;

class Player{

    public:
        Player();
        ~Player();
        //moving
        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();
        //rotating
        void rotateLeft();
        void rotateRight();

        //gettery
        double getPositionX();
        double getPositionY();
        float getAngle();
        float getRadius();
        sf::CircleShape getPlayerDot();
        sf::VertexArray getDirectionLine();


        bool isMovingUp, isMovingDown, isMovingLeft, isMovingRight;
        bool isRotatingLeft, isRotatingRight;

    private:

        float radius;
        double positionX;
        double positionY;

        
        
        double velocity;
        float angle;

        sf::Color playerColor;
        sf::CircleShape playerDot;

        sf::Vector2f direction;
        sf::VertexArray directionLine;

};

#endif // PLAYER_H