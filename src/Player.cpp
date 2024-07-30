#include "../include/Player.h"
#include <cmath>

Player::Player() {
  std::clog << "Utworzenie obiektu player" << std::endl;

  positionX = 0.0;
  positionY = 0.0;
  velocity = 0.0;
  angle = 0.0;

  isMovingForwards = isMovingBackwards = isMovingLeft = isMovingRight = false;
  isRotatingLeft = isRotatingRight = false;
  isSprinting = false;

  speed = PLAYER_DEFAULT_SPEED;
  rotationSpeed = M_PI / 180.f * DEFAULT_ROTATION_SPEED;
}

Player::~Player() {}
