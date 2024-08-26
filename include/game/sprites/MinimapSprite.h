#pragma once
#include "../components/TransformComponent.h"
#include <SFML/Graphics.hpp>

class MinimapSprite : public sf::Drawable {
public:
  virtual void update(TransformComponent transform) = 0;

private:
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const = 0;
};
