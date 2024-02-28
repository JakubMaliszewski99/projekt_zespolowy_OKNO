#include "..\include\Game.h"

        //Konstruktor
Game::Game(){

    std::clog << "Utworzenie obiektu game" << std::endl;

    m_is_running = true;
    window_width = 1000;
    window_height = 900;
    frames_per_second = 60;
}

// Destruktor
Game::~Game() {

    std::clog  << "Destrukcja obiektu game" << std::endl;
}

// Główna metoda z pętlą gry
void Game::run(){
    std::clog << "Gra chodzi a nawet biegnie" << std::endl;

    this->init();


    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear();
        m_window.display();
    }

    std::clog << "Gra już nie biega" << std::endl;
    this->cleanup();
}

// Inicjalizacja
void Game::init() {
    
    std::clog << "Inicjalizacja gry" << std::endl;
    
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
            m_window.close();
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
