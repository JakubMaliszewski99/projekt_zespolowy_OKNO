#pragma once
#include <SFML/Graphics.hpp>
#include "../components/TransformComponent.h"

class MinimapSprite : public sf::Drawable {
public:
  virtual void update(TransformComponent transform) = 0;
private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};