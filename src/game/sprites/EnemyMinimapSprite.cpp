#include "../include/game/sprites/EnemyMinimapSprite.h"

EnemyMinimapSprite::EnemyMinimapSprite(sf::Color color, bool isFovVisible) {
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


  m_fovArc.setPrimitiveType(sf::LineStrip); // <-- Initialize the arc
  m_fovArc.resize(numArcPoints + 1);        // <-- Set the number of points


  this->m_isFovVisible = isFovVisible;
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
  float fov = FOV_ENEMIES * (M_PI / 180);
  float hFov = fov / 2;
  sf::Vector2f directionRay1 =
      sf::Vector2f(std::cos(enemyAngle - hFov), std::sin(enemyAngle - hFov));
  sf::Vector2f directionRay2 =
      sf::Vector2f(std::cos(enemyAngle + hFov), std::sin(enemyAngle + hFov));

  m_fovRays[0][0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_fovRays[0][1].position = m_fovRays[0][0].position + directionRay1 * FOV_LENGTH;
  m_fovRays[1][0].position =
      m_enemyDot.getPosition() +
      sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  m_fovRays[1][1].position = m_fovRays[1][0].position + directionRay2 * FOV_LENGTH;


sf::Vector2f originPosition = m_enemyDot.getPosition() + sf::Vector2f(m_enemyDot.getRadius(), m_enemyDot.getRadius());
  for (int i = 0; i <= numArcPoints; ++i) {       
      float t = static_cast<float>(i) / numArcPoints;
      float angle = enemyAngle - hFov + t * fov;  // Interpolate between the start and end angles
      sf::Vector2f pointOnArc = originPosition + sf::Vector2f(std::cos(angle), std::sin(angle)) * FOV_LENGTH;
      m_fovArc[i].position = pointOnArc;
      m_fovArc[i].color = sf::Color::Yellow; // Set color for the arc
  }


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
    target.draw(m_fovArc);
    
  }
  target.draw(m_velocityLine);
}
