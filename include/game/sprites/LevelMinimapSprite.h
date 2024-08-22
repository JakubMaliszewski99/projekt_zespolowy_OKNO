#pragma once
#include "../include/core/engine/GameLevel.h"
#include "MinimapSprite.h"

class LevelMinimapSprite : public MinimapSprite {
public:
  LevelMinimapSprite(GameLevel *level);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  sf::VertexArray map;
};