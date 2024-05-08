#pragma once
#include <SFML/Graphics.hpp>

struct TransformComponent {
  float positionX;
  float positionY;
  float positionZ;
  float targetPositonZ;
  sf::Vector2f velocity;
  float angle;
};