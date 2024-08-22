#pragma once
#include <SFML/Graphics.hpp>

struct TransformComponent {
  float positionX;
  float positionY;
  float positionZ;
  float height;
  sf::Vector2f velocity;
  float velocityZ;
  sf::Vector2f acceleration;
  float angle;
};