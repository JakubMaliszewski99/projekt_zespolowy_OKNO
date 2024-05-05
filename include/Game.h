#ifndef GAME_H
#define GAME_H

#include "../include/Enemy.h"
#include "../include/Menu.h"
#include "../include/Painter2D.h"
#include "../include/Player.h"
#include "../include/core/loaders/WADLoader.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

class Game {
public:
  Game();
  ~Game();

  void init();
  void run();
  void processEvents();
  void handlePlayerInput();
  void update(sf::Time deltaTime);
  void render();
  void cleanup();
  bool isRunning() const;

private:
  sf::RenderWindow m_window;
  int m_windowWidth;
  int m_windowHeight;
  int m_framesPerSecond;
  bool m_isRunning;
  Player player;
  Enemy enemy;
  GameLevel *m_level;
  sf::VertexArray m_map;
  sf::View m_camera;

  Painter2D renderer;
};

#endif // GAME_H