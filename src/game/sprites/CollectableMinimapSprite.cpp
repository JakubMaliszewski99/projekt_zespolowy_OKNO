#include "../include/game/sprites/CollectableMinimapSprite.h"
#include "../include/core/core.h"

CollectableMinimapSprite::CollectableMinimapSprite(
    sf::Color color, const std::shared_ptr<ECSManager> &m_ecsManager)
    : m_ecsManager(m_ecsManager) {
  collectableDot.setFillColor(color);
  collectableDot.setRadius(2.0f);
  collectableDot.setOrigin(10.0f, 10.0f);
}

void CollectableMinimapSprite::update(TransformComponent transform) {
  collectableDot.setPosition(
      sf::Vector2f(transform.positionX, transform.positionY));

  auto &playerTransform = m_ecsManager->getComponent<TransformComponent>(0);

  sf::Vector2f spriteCenter(transform.positionX, transform.positionY);
  sf::Vector2f playerCenter(playerTransform.positionX,
                            playerTransform.positionY);
  sf::Vector2f displacement = playerCenter - spriteCenter;

  float disAngle = atan2f(displacement.y, displacement.x) + M_PI / 4;

  sf::Vector2f v1;
  sf::Vector2f v2;

  v1.x = 32 * cosf(disAngle) - 32 * sinf(disAngle);
  v1.y = 32 * sinf(disAngle) + 32 * cosf(disAngle);
  v2.x = -32 * cosf(disAngle) + 32 * sinf(disAngle);
  v2.y = -32 * sinf(disAngle) - 32 * cosf(disAngle);

  v1 += spriteCenter;
  v2 += spriteCenter;

  line[0] = v1;
  line[1] = v2;
}

void CollectableMinimapSprite::draw(sf::RenderTarget &target,
                                    sf::RenderStates states) const {
  // target.draw(collectableDot);
  target.draw(line, 2, sf::Lines);
}
