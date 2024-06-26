#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Menu.h"
#include "../include/Painter2D.h"
#include "../include/CollisionSolver.h"

#define _USE_MATH_DEFINES
#include <cmath>

const int WINDOW_X_SIZE = 1000;
const int WINDOW_Y_SIZE = 600;

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
    void processEvents();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void update(sf::Time deltaTime);
    void render();
    void cleanup();
    bool isRunning() const;

private:
        void resolveCollisions();

        sf::RenderWindow m_window;
        int window_width;
        int window_height;
        int frames_per_second;
        bool m_is_running;
        
        Player player;
        Enemy enemy;

        Painter2D renderer;
        CollisionSolver collisionSolver;
};

#endif // GAME_H