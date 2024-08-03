#pragma once
#include "MinimapSprite.h"
#include "../include/core/core.h"

class EnemyMinimapSprite : public MinimapSprite {
public:
  EnemyMinimapSprite(sf::Color color);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  float m_radius;
  sf::CircleShape m_enemyDot;
  sf::Color m_enemyColor;
  sf::Vector2f m_direction;
  sf::Vector2f m_velocity;
  sf::VertexArray m_directionLine;
  sf::VertexArray m_velocityLine;
  sf::VertexArray m_fovRays[2];
  bool m_isFovVisible;
};