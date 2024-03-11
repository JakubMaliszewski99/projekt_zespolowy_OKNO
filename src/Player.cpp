#include "..\include\Player.h"

//Konstruktor
Player::Player(){
    std::clog << "Utworzenie obiektu player" << std::endl;

    radius = 10.0f;
    positionX = 0.0;
    positionY = 0.0;
    playerColor = sf::Color::White;

    /* Może ruch jako boole?
    isMovingUp = false;
    isMovingDown = false;
    isMovingLeft = false;
    isMovingRight = false;
    */

    playerDot.setRadius(radius);
    playerDot.setPosition(positionX, positionY);
    playerDot.setFillColor(playerColor);
}

//Destruktor
Player::~Player(){}

//Ruch na minimapce
void Player::moveUp(){
    positionY -= PLAYER_MOV;
    playerDot.setPosition(positionX, positionY);
}

void Player::moveDown(){
    positionY += PLAYER_MOV;
    playerDot.setPosition(positionX, positionY);
}

void Player::moveRight(){
    positionX += PLAYER_MOV;
    playerDot.setPosition(positionX, positionY);
}

void Player::moveLeft(){
    positionX -= PLAYER_MOV;
    playerDot.setPosition(positionX, positionY);
}

//Gettery
sf::CircleShape Player::getPlayerDot(){
    return playerDot;
}