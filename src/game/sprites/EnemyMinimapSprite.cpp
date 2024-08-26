#include "../include/game/sprites/EnemyMinimapSprite.h"

EnemyMinimapSprite::EnemyMinimapSprite(sf::Color color) {
  m_radius = 5.0f;
  m_enemyColor = sf::Color::Cyan;

  m_enemyDot.setFillColor(color);
  m_enemyDot.setRadius(m_radius);

  m_directionLine.setPrimitiveType(sf::Lines);
  m_directionLine.resize(2);
  m_directionLine[0].color = sf::Color::Red;
  m_directionLine[1].color = sf::Color::Red;

  m_velocityLine.setPrimitiveType(sf::Lines);
  m_velocityLine.resize(2);
  m_velocityLine[0].color = sf::Color::Green;
  m_velocityLine[1].color = sf::Color::Green;

  m_fovRays[0].setPrimitiveType(sf::Lines);
  m_fovRays[0].resize(2);
  m_fovRays[0][0].color = sf::Color::Yellow;
  m_fovRays[0][1].color = sf::Color::Yellow;
  m_fovRays[1].setPrimitiveType(sf::Lines);
  m_fovRays[1].resize(2);
  m_fovRays[1][0].color = sf::Color::Yellow;
  m_fovRays[1][1].color = sf::Color::Yellow;

  this->m_isFovVisible = m_isFovVisible;
}

void EnemyMinimapSprite::update(TransformComponent transform) {
  m_enemyDot.setPosition(
      sf::Vector2f(transform.positionX, transform.positionY));

  float enemyAngle = transform.angle;
  m_direction = sf::Vector2f(std::cos(enemyAngle), std::sin(enemyAngle));
  m_directionLine[0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_directionLine[1].position =
      m_directionLine[0].position + m_direction * 18.f;
  float fov = FOV * (M_PI / 180);
  float hFov = fov / 2;
  sf::Vector2f directionRay1 =
      sf::Vector2f(std::cos(enemyAngle - hFov), std::sin(enemyAngle - hFov));
  sf::Vector2f directionRay2 =
      sf::Vector2f(std::cos(enemyAngle + hFov), std::sin(enemyAngle + hFov));

  m_fovRays[0][0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_fovRays[0][1].position = m_fovRays[0][0].position + directionRay1 * 2000.0f;
  m_fovRays[1][0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_fovRays[1][1].position = m_fovRays[1][0].position + directionRay2 * 2000.0f;

  m_velocity = transform.velocity;
  m_velocityLine[0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_velocityLine[1].position = m_velocityLine[0].position + m_velocity * 20.0f;
}

void EnemyMinimapSprite::draw(sf::RenderTarget &target,
                              sf::RenderStates states) const {
  target.draw(m_enemyDot);
  target.draw(m_directionLine);
  if (m_isFovVisible) {
    target.draw(m_fovRays[0]);
    target.draw(m_fovRays[1]);
  }
  target.draw(m_velocityLine);
}
