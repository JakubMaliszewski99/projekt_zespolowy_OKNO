#include "../include/Game.h"
#include "core/engine/InputManager.h"
#include "core/logger/Logger.h"
#include <cmath>

Game::Game() {
  m_isRunning = true;
  m_windowWidth = 1500;
  m_windowHeight = 900;
  m_framesPerSecond = 60;

  WADLoader loader;
  m_level = loader.loadFromFile("../data/assets/DOOM.WAD", "E1M1");
}

Game::~Game() {}

void Game::run() {
  init();
  sf::Clock clock;

  while (m_window.isOpen()) {
    sf::Time deltaTime = clock.restart();
    processEvents();
    update(deltaTime);
    render();
  }

  cleanup();
}

void Game::init() {
  m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight),
                  "2.5D FPS Game");
  m_map = sf::VertexArray(sf::Lines);
  for (auto linedef : m_level->linedefs) {
    mapvertexes_t start = m_level->vertexes[linedef.startVertex];
    mapvertexes_t end = m_level->vertexes[linedef.endVertex];

    m_map.append(sf::Vertex(sf::Vector2f((float)start.x, (float)start.y),
                          sf::Color::Red));
    m_map.append(
        sf::Vertex(sf::Vector2f((float)end.x, (float)end.y), sf::Color::Red));
  }

  for (auto thing : m_level->things) {
    if (thing.type == 1) {
      player.setPosition(sf::Vector2f((float)thing.x, (float)thing.y));
      m_camera.setCenter(sf::Vector2f((float)thing.x, (float)thing.y));
    }
  }
}

void Game::processEvents() {
  sf::Event event;
  handlePlayerInput();
  while (m_window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      m_isRunning = false;
      m_window.close();
      break;
    }
  }
}

void Game::handlePlayerInput() {
  player.isMovingForwards =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::W);
  player.isMovingBackwards =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::S);
  player.isMovingLeft =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::A);
  player.isMovingRight =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::D);
  player.isRotatingRight =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::E);
  player.isRotatingLeft =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::Q);
  player.isSprinting =
      InputManager::getInstance()->isKeyPressed(sf::Keyboard::LShift);
}

void Game::update(sf::Time deltaTime) {

  sf::Vector2f movement(0.f, 0.f);
  float velocity = player.getSpeed() * deltaTime.asSeconds();
  float rotation = 0.f;
  player.isSprinting ? player.setSpeed(PLAYER_DEFAULT_SPEED * 2)
                     : player.setSpeed(PLAYER_DEFAULT_SPEED);
  if (player.isMovingForwards) {
    movement.x += cos(player.getAngle()) * velocity;
    movement.y += sin(player.getAngle()) * velocity;
  }
  if (player.isMovingBackwards) {
    movement.x -= cos(player.getAngle()) * velocity;
    movement.y -= sin(player.getAngle()) * velocity;
  }
  if (player.isMovingLeft) {
    movement.x -= cos(player.getAngle() + 90 * (M_PI / 180)) * velocity;
    movement.y -= sin(player.getAngle() + 90 * (M_PI / 180)) * velocity;
  }
  if (player.isMovingRight) {
    movement.x += cos(player.getAngle() + 90 * (M_PI / 180)) * velocity;
    movement.y += sin(player.getAngle() + 90 * (M_PI / 180)) * velocity;
  }
  if (player.isRotatingRight) {
    rotation += player.getRotationSpeed() * deltaTime.asSeconds();
  }
  if (player.isRotatingLeft) {
    rotation -= player.getRotationSpeed() * deltaTime.asSeconds();
  }

  m_camera.move(movement);
  player.move(movement);
  player.rotate(rotation);
}

void Game::render() {
  m_window.clear();

  m_window.setView(m_camera);
  renderer.draw(&player, &m_window);
  m_window.draw(m_map);

  m_window.display();
}

void Game::cleanup() {}

bool Game::isRunning() const { return m_isRunning; }
