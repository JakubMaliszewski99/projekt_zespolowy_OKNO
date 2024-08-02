#include "../include/game/sprites/EnemyMinimapSprite.h"

EnemyMinimapSprite::EnemyMinimapSprite(sf::Color color) {
  enemyDot.setFillColor(color);
  enemyDot.setRadius(10.0f);
  enemyDot.setOrigin(5.0f, 5.0f);
}

void EnemyMinimapSprite::update(TransformComponent transform) {
  enemyDot.setPosition(sf::Vector2f(transform.positionX, transform.positionY));
}

void EnemyMinimapSprite::draw(sf::RenderTarget &target,
                                    sf::RenderStates states) const {
  target.draw(enemyDot);
}