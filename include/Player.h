#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include <math.h>

//stałe
const double PLAYER_MOV = 10.0;
//const double M_PI = cos(1)*4;

class Player{

    public:
        Player();
        ~Player();
        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();
        //gettery
        sf::CircleShape getPlayerDot();
        float getAngle();

    private:

        float radius;
        double positionX;
        double positionY;
        
        double velocity;
        float angle;

        /* może ruch jako boole
        bool isMovingUp;
        bool isMovingDown;
        bool isMovingLeft;
        bool isMovingRight;
        */
        sf::Color playerColor;
        sf::CircleShape playerDot;
        
        //static sf::Vertex playerDirectionLine[2];

};

#endif // PLAYER_H