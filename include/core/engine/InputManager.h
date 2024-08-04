#pragma once
#include <SFML/Graphics.hpp>

// TODO: Refactor to EventManager?
class InputManager {
public:
  InputManager();
  static InputManager *getInstance();
  bool isKeyPressed(sf::Keyboard::Key key);
  bool isMouseButtonPressed(sf::Mouse::Button button);
private:
  static InputManager *m_instance;
};