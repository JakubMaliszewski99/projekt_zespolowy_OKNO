#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// TODO: Refactor to EventManager?
class InputManager {
public:
  InputManager();
  static InputManager *getInstance();
  bool isKeyPressed(sf::Keyboard::Key key);
  bool isMouseButtonPressed(sf::Mouse::Button button);
  sf::Vector2i getMousePosition(std::shared_ptr<sf::RenderWindow> window);
  sf::Vector2i getMouseDelta();
  void setMouseDelta(sf::Vector2i mouseDelta);

private:
  static InputManager *m_instance;
  sf::Vector2i m_mouseDelta;
};
