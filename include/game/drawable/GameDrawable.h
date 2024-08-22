#pragma once
#include "../components/TransformComponent.h"
#include "core/engine/GameLevel.h"
#include <SFML/Graphics.hpp>

class GameDrawable : public sf::Drawable {
public:
  virtual void update(TransformComponent transform) = 0;

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) = 0;
};