#pragma once
#include "MinimapSprite.h"

class CollectableMinimapSprite : public MinimapSprite {
public:
  CollectableMinimapSprite(sf::Color color);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  sf::CircleShape collectableDot;
};