#pragma once
#include <SFML/Graphics.hpp>
#include "MinimapSprite.h"

class PlayerMinimapSprite : public MinimapSprite {
public:
  PlayerMinimapSprite(bool isFovVisiable);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  
  // TODO: Change names, add prefix m_
  float radius;
  sf::Color playerColor;
  sf::CircleShape playerDot;
  sf::Vector2f direction;
  sf::VertexArray directionLine;
  sf::VertexArray fovRays[2];
  bool isFovVisible;
};