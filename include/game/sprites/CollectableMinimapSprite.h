#pragma once
#include "../include/core/ecs/ECSManager.h"
#include "MinimapSprite.h"

class CollectableMinimapSprite : public MinimapSprite {
public:
  CollectableMinimapSprite(sf::Color color,
                           const std::shared_ptr<ECSManager> &m_ecsManager);
  void update(TransformComponent transform);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  sf::CircleShape collectableDot;
  sf::Vertex line[2];
  std::shared_ptr<ECSManager> m_ecsManager;
};
