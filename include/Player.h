#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SFML/Graphics.hpp>

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
        sf::Color playerColor;
        sf::CircleShape playerDot;

};

#endif // PLAYER_H