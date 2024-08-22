#pragma once
#include "../../core/math/utilities.h"
#include "../../core/ecs/ECSManager.h"
#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"
#include "../components/ControllableComponent.h"
#include "../components/MinimapSpriteComponent.h"
#include "../components/TransformComponent.h"
#include <cmath>

#define M_PI 3.14159265358979323846

const float DEFAULT_SPEED = 100.f;
const float PLAYER_DEFAULT_SPEED = 150.0f;
const float ENEMY_DEFAULT_SPEED = 70.f;
const float DEFAULT_ROTATION_SPEED = 10;

class PlayerControllSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager) { m_manager = manager; }

  void update(float dt) {
    for (auto const &entity : m_entities) {
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &controllable =
          m_manager->getComponent<ControllableComponent>(entity);
      auto &drawable = m_manager->getComponent<MinimapSpriteComponent>(entity);

      if (!controllable.isPlayer) {
        continue;
      }

      sf::Vector2f movement(0.f, 0.f);
      float velocity = PLAYER_DEFAULT_SPEED * dt;
      float rotation = 0.f;

      // TODO: Fix diagonal velocity problem
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::W)) {
        movement.x += cos(transform.angle) * velocity;
        movement.y += sin(transform.angle) * velocity;
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::S)) {
        movement.x -= cos(transform.angle) * velocity;
        movement.y -= sin(transform.angle) * velocity;
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::D)) {
        movement.x -= cos(transform.angle + 90 * (M_PI / 180)) * velocity;
        movement.y -= sin(transform.angle + 90 * (M_PI / 180)) * velocity;
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::A)) {
        movement.x += cos(transform.angle + 90 * (M_PI / 180)) * velocity;
        movement.y += sin(transform.angle + 90 * (M_PI / 180)) * velocity;
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Q)) {
        rotation += DEFAULT_ROTATION_SPEED * dt * 0.4f;
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::E)) {
        rotation -= DEFAULT_ROTATION_SPEED * dt * 0.4f;
      }
      // TODO: FIX ZOOMING - caused by setting size (flipping Y)
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Dash)) {
        drawable.camera.zoom(1.001f);
      }
      if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Equal)) {
        drawable.camera.zoom(0.999f);
      }

      transform.velocity = movement;
      transform.angle = normalizeRadianAngle(transform.angle + rotation);
    }
  }

private:
  std::shared_ptr<ECSManager> m_manager;
};