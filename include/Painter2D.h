#ifndef PAINTER2D_H
#define PAINTER2D_H

#pragma once

#include "../include/Enemy.h"
#include "../include/Player.h"
#include <SFML/Graphics.hpp>

class Painter2D {
public:
  Painter2D();
  ~Painter2D();

  void draw(Player *player, sf::RenderWindow *window);
  void draw(Enemy *enemy);

private:
  float radius;
  sf::Color playerColor;
  sf::CircleShape playerDot;

  sf::Vector2f direction;
  sf::VertexArray directionLine;
};

#endif