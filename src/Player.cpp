#include "..\include\Player.h"

//Konstruktor
Player::Player(){

    std::clog << "Utworzenie obiektu player" << std::endl;

    radius = 10.0f;
    positionX = 0.0;
    positionY = 0.0;
    playerColor = sf::Color::White;

    playerDot.setRadius(radius);
    playerDot.setPosition(positionX, positionY);
    playerDot.setFillColor(playerColor);

}

//Destruktor
Player::~Player(){

}

void Player::moveUp(){

}

void Player::moveDown(){

}

void Player::moveRight(){

}

void Player::moveLeft(){

}


sf::CircleShape Player::getPlayerDot(){
    return playerDot;
}

