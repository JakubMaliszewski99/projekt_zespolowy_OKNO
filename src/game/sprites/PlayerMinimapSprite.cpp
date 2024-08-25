#include "../include/game/sprites/PlayerMinimapSprite.h"
#include "../include/core/core.h"

PlayerMinimapSprite::PlayerMinimapSprite(bool isFovVisible) {
  radius = 5.0f;
  playerColor = sf::Color::White;

  playerDot.setRadius(radius);
  playerDot.setFillColor(playerColor);

  directionLine.setPrimitiveType(sf::Lines);
  directionLine.resize(2);
  directionLine[0].color = sf::Color::Red;
  directionLine[1].color = sf::Color::Red;

  velocityLine.setPrimitiveType(sf::Lines);
  velocityLine.resize(2);
  velocityLine[0].color = sf::Color::Green;
  velocityLine[1].color = sf::Color::Green;

  fovRays[0].setPrimitiveType(sf::Lines);
  fovRays[0].resize(2);
  fovRays[0][0].color = sf::Color::Yellow;
  fovRays[0][1].color = sf::Color::Yellow;
  fovRays[1].setPrimitiveType(sf::Lines);
  fovRays[1].resize(2);
  fovRays[1][0].color = sf::Color::Yellow;
  fovRays[1][1].color = sf::Color::Yellow;

  this->isFovVisible = isFovVisible;
}

void PlayerMinimapSprite::update(TransformComponent transform) {
  playerDot.setPosition(transform.positionX, transform.positionY);

  float playerAngle = transform.angle;
  direction = sf::Vector2f(std::cos(playerAngle), std::sin(playerAngle));
  directionLine[0].position =
      playerDot.getPosition() +
      sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
  directionLine[1].position = directionLine[0].position + direction * 18.f;
  float fov = FOV * (M_PI / 180);
  float hFov = fov / 2;
  sf::Vector2f directionRay1 =
      sf::Vector2f(std::cos(playerAngle - hFov), std::sin(playerAngle - hFov));
  sf::Vector2f directionRay2 =
      sf::Vector2f(std::cos(playerAngle + hFov), std::sin(playerAngle + hFov));

  fovRays[0][0].position =
      playerDot.getPosition() +
      sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
  fovRays[0][1].position = fovRays[0][0].position + directionRay1 * 700.0f;
  fovRays[1][0].position =
      playerDot.getPosition() +
      sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
  fovRays[1][1].position = fovRays[1][0].position + directionRay2 * 700.0f;

  velocity = transform.velocity;
  velocityLine[0].position =
      playerDot.getPosition() +
      sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
  velocityLine[1].position = velocityLine[0].position + velocity * 20.0f;
}

void PlayerMinimapSprite::draw(sf::RenderTarget &target,
                               sf::RenderStates states) const {
  target.draw(playerDot);
  target.draw(directionLine);
  if (isFovVisible) {
    target.draw(fovRays[0]);
    target.draw(fovRays[1]);
  }
  target.draw(velocityLine);
}
