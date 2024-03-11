#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>

//stałe
const double PLAYER_MOV = 10.0;

class Player{

    public:
        Player();
        ~Player();
        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();
        sf::CircleShape getPlayerDot();

    private:

        float radius;
        double positionX;
        double positionY;
        /* może ruch jako boole
        bool isMovingUp;
        bool isMovingDown;
        bool isMovingLeft;
        bool isMovingRight;
        */
        sf::Color playerColor;
        sf::CircleShape playerDot;

};

#endif // PLAYER_H