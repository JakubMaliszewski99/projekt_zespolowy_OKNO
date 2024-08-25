#pragma once
#include "../../core/ecs/ECSManager.h"
#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"
#include "../../core/math/utilities.h"
#include "../components/ControllableComponent.h"
#include "../components/MinimapSpriteComponent.h"
#include "../components/PlayerStateComponent.h"
#include "../components/TransformComponent.h"
#include <cmath>

class PlayerControllSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager,
            std::shared_ptr<sf::RenderWindow> window) {
    m_manager = manager;
    m_window = window;
  }

  void initializeMouse(std::shared_ptr<sf::RenderWindow> window) {
    sf::Mouse::setPosition(sf::Vector2i(m_window->getSize() / 2u), *m_window);
    currentMousePosition =
        InputManager::getInstance()->getMousePosition(m_window);
    lastMousePosition = currentMousePosition;
  }

  void update(float dt) {
    for (auto const &entity : m_entities) {
      auto &controllable =
          m_manager->getComponent<ControllableComponent>(entity);
      auto &state = m_manager->getComponent<PlayerStateComponent>(entity);

      if (!controllable.isPlayer || !m_window->hasFocus()) {
        continue;
      }

      // Myszka
      currentMousePosition =
          InputManager::getInstance()->getMousePosition(m_window);
      mouseDelta = currentMousePosition - lastMousePosition;
      InputManager::getInstance()->setMouseDelta(mouseDelta);
      sf::Mouse::setPosition(sf::Vector2i(m_window->getSize() / 2u), *m_window);
      lastMousePosition =
          InputManager::getInstance()->getMousePosition(m_window);

      if (state.isAlive) {

        state.isMovingForward =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::W) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Up) ||
             mouseDelta.y < 0);
        state.isMovingBackwards =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::S) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Down) ||
             mouseDelta.y > 0);
        state.isMovingRight =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::D) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Period) ||
             (mouseDelta.x > 0 &&
              InputManager::getInstance()->isMouseButtonPressed(
                  sf::Mouse::Right)));
        state.isMovingLeft =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::A) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Comma) ||
             (mouseDelta.x < 0 &&
              InputManager::getInstance()->isMouseButtonPressed(
                  sf::Mouse::Right)));
        state.isRotatingLeft =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Q) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Left) ||
             (mouseDelta.x < 0 &&
              !InputManager::getInstance()->isMouseButtonPressed(
                  sf::Mouse::Right)));
        state.isRotatingRight =
            (InputManager::getInstance()->isKeyPressed(sf::Keyboard::E) ||
             InputManager::getInstance()->isKeyPressed(sf::Keyboard::Right) ||
             (mouseDelta.x > 0 &&
              !InputManager::getInstance()->isMouseButtonPressed(
                  sf::Mouse::Right)));

        // TODO: Implementacja tego w Systemie do renderowania
        state.isZoomingIn =
            InputManager::getInstance()->isKeyPressed(sf::Keyboard::Dash);
        state.isZoomingOut =
            InputManager::getInstance()->isKeyPressed(sf::Keyboard::Equal);

        // TODO: Implementacja w Systemie broni
        state.isFiring =
            InputManager::getInstance()->isKeyPressed(sf::Keyboard::LControl);
      }
    }
  }

private:
  std::shared_ptr<ECSManager> m_manager;
  std::shared_ptr<sf::RenderWindow> m_window;
  sf::Vector2i lastMousePosition;
  sf::Vector2i currentMousePosition;
  sf::Vector2i mouseDelta;
};
