#include "../include/game/sprites/CollectableMinimapSprite.h"

CollectableMinimapSprite::CollectableMinimapSprite(sf::Color color) {
  collectableDot.setFillColor(color);
  collectableDot.setRadius(20.0f);
  collectableDot.setOrigin(10.0f, 10.0f);
}

void CollectableMinimapSprite::update(TransformComponent transform) {
  collectableDot.setPosition(sf::Vector2f(transform.positionX, transform.positionY));
}

void CollectableMinimapSprite::draw(sf::RenderTarget &target,
                                    sf::RenderStates states) const {
  target.draw(collectableDot);
}
