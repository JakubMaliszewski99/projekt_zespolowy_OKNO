#include "..\include\Player.h"
// Konstruktor
Player::Player(){
    std::clog << "Utworzenie obiektu player" << std::endl;

    // Inicjalizacja zmiennych
    radius = 5.0f;
    positionX = 0.0;
    positionY = 0.0;
    velocity = 0.0;

    isMovingForwards = isMovingBackwards = isMovingLeft = isMovingRight = false;
    isRotatingLeft = isRotatingRight = false;
    isSprinting = false;
    
    playerColor = sf::Color::White;

    // Kropka oznaczająca playera
    playerDot.setRadius(radius);
    playerDot.setPosition(positionX, positionY);
    playerDot.setFillColor(playerColor);

    // Linia wskazująca kierunek patrzenia
    directionLine.setPrimitiveType(sf::Lines);
    directionLine.resize(2);
    directionLine[0].color = sf::Color::Red;
    directionLine[1].color = sf::Color::Red;

    angle = playerDot.getRotation() * M_PI/180.f;
    direction = sf::Vector2f(std::cos(angle), std::sin(angle));
    directionLine[0].position = playerDot.getPosition() + sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
    directionLine[1].position = directionLine[0].position + direction * 18.f;

    speed = PLAYER_DEFAULT_SPEED;
    rotationSpeed = M_PI/180.f * 500;
}

// Destruktor
Player::~Player(){}

// Ruch na minimapce
void Player::move(sf::Vector2f movement){
    positionX += movement.x;
    positionY += movement.y;
    playerDot.setPosition(positionX, positionY);
    directionLine[0].position = playerDot.getPosition() + sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
    directionLine[1].position = directionLine[0].position + direction * 15.f;
}

// Rotacja
void Player::rotate(float rotation){
    angle += rotation;
    direction = sf::Vector2f(std::cos(angle), std::sin(angle));
    directionLine[0].position = playerDot.getPosition() + sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
    directionLine[1].position = directionLine[0].position + direction * 15.f;
}

//Gettery

double Player::getPositionX(){
    return positionX;
}
double Player::getPositionY(){
    return positionY;
}

sf::CircleShape Player::getPlayerDot(){
    return playerDot;
}

sf::VertexArray Player::getDirectionLine(){
    return directionLine;
}

float Player::getAngle(){
    return angle;
}

float Player::getRadius(){
    return radius;
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