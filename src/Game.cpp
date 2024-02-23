#include "..\include\Game.h"

// Konstruktor
Game::Game(){
    m_is_running = true;
    window_width = 1000;
    window_height = 900;
    frames_per_second = 60;

}

// Destruktor
Game::~Game() {
}

// Inicjalizacja
void Game::init() {
    
    // Stworzenie okna gry
    m_window.create(sf::VideoMode(window_width, window_height), "2.5D FPS Game");

    // Limit klatek na sekundę
    m_window.setVerticalSyncEnabled(true);
    m_window.setFramerateLimit(frames_per_second);
}

// Eventy
void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {

        // Wyjście z gry
        if (event.type == sf::Event::Closed) {
            m_is_running = false;
        }
    }

}

// Logika gry
void Game::update() {

}

// Renderer
void Game::render() {
    m_window.clear();

    m_window.display();
}

// Cleanup
void Game::cleanup() {
}

// Sprawdź czy gra jest on czy off
bool Game::isRunning() const {
    return m_is_running;
}
