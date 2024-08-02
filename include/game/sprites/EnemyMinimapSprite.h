#pragma once
#include "MinimapSprite.h"

class EnemyMinimapSprite : public MinimapSprite {
public:
  EnemyMinimapSprite(sf::Color color);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  sf::CircleShape enemyDot;
};