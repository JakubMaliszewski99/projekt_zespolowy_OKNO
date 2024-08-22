#pragma once
#include "GameDrawable.h"

class LevelDrawable : public GameDrawable {
public:
  LevelDrawable();
  virtual void update(TransformComponent palyerTransform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states);
};
