#include "../include/core/engine/InputManager.h"

InputManager *InputManager::m_instance = nullptr;

InputManager::InputManager() {
  
}

InputManager *InputManager::getInstance() {
  if (!m_instance) {
    m_instance = new InputManager();
  }

  return m_instance;
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) { 
  return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button){
  return sf::Mouse::isButtonPressed(button);
}
