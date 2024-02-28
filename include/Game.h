#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
    void processEvents();
    void update();
    void render();
    void cleanup();
    bool isRunning() const;

private:
        sf::RenderWindow m_window;
        int window_width;
        int window_height;
        int frames_per_second;
        bool m_is_running;
        sf::CircleShape mPlayer;
};

#endif // GAME_H