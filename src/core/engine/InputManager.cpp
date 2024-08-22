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

sf::Vector2i InputManager::getMousePosition(std::shared_ptr<sf::RenderWindow> window){
  return sf::Mouse::getPosition(*window);
}

sf::Vector2i InputManager::getMouseDelta(){
  return m_mouseDelta;
}

void InputManager::setMouseDelta(sf::Vector2i mouseDelta){
  this->m_mouseDelta = mouseDelta;
}
