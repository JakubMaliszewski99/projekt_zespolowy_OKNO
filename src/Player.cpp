#include "../include/Player.h"
// Konstruktor
Player::Player(CollisionSolver* newSolver): Actor(newSolver)
{
    std::clog << "Utworzenie obiektu player" << std::endl;

    // Inicjalizacja zmiennych
    positionX = 0.0;
    positionY = 0.0;
    velocity = 0.0;
    angle = 0.0;

    isMovingForwards = isMovingBackwards = isMovingLeft = isMovingRight = false;
    isRotatingLeft = isRotatingRight = false;
    isSprinting = false;

    speed = PLAYER_DEFAULT_SPEED;
    rotationSpeed = M_PI/180.f * DEFAULT_ROTATION_SPEED;
}

// Destruktor
Player::~Player(){}
