#include "..\include\Game.h"


// Konstruktor
Game::Game(){

    std::clog << "Utworzenie obiektu game" << std::endl;

    m_is_running = true;
    window_width = 1500;
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
    init();
    sf::Clock clock;
    
/*
    Menu menu;

        while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
            // Pass the event to the menu for handling
            menu.handleEvent(event, m_window);
        }

        m_window.clear();
        // Draw the menu
        menu.draw(m_window);
        m_window.display();
    }
    */

    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }

    std::clog << "Gra już nie biega" << std::endl;
    cleanup();
}

// Inicjalizacja
void Game::init() {
    
    std::clog << "Inicjalizacja gry" << std::endl;
    
    // Stworzenie okna gry
    m_window.create(sf::VideoMode(window_width, window_height), "2.5D FPS Game");

    /*
    // Limit klatek na sekundę
    m_window.setVerticalSyncEnabled(true);
    m_window.setFramerateLimit(frames_per_second);
    */
}

// Eventy
void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type)
        {
            // Naciśnięto klawisz
            case sf::Event::KeyPressed:
                std::clog << "Nacisnąłeś klawisz: ";
                handlePlayerInput(event.key.code, true);
                break;
            
            // Puszczono klawisz
            case sf::Event::KeyReleased:
                std::clog << "Puściłeś klawisz: ";
                handlePlayerInput(event.key.code, false);
                break;
            
            // Wyjście z gry
            case sf::Event::Closed:
                m_is_running = false;
                m_window.close();
                break;
        }
    }

}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {

    switch (key)
    {
        // Ruch
        case sf::Keyboard::W:
            std::clog << "W" << std::endl;
            player.isMovingUp = isPressed;
            break;
        case sf::Keyboard::S:
            std::clog << "S" << std::endl;
            player.isMovingDown = isPressed;
            break;
        case sf::Keyboard::A:
            std::clog << "A" << std::endl;
            player.isMovingLeft = isPressed;
            break;
        case sf::Keyboard::D:
            std::clog << "D" << std::endl;
            player.isMovingRight = isPressed;
            break;
        // Rotacja
        case sf::Keyboard::E:
            std::clog << "E" << std::endl;
            player.isRotatingRight = isPressed;
            break;
        case sf::Keyboard::Q:
            std::clog << "Q" << std::endl;
            player.isRotatingLeft = isPressed;
            break;
        // Bieg
        // Strzelanie
        // Interakcja
    }
}
// Logika gry
void Game::update(sf::Time deltaTime) {

    sf::Vector2f movement(0.f, 0.f);
    float rotation = 0.f;
    if(player.isMovingUp && player.getPositionY() > 0){
        movement.y -= player.getSpeed();
    }
    if(player.isMovingDown && player.getPositionY() < window_height - (player.getPlayerDot().getRadius()) * 2){
        movement.y += player.getSpeed();
    }
    if(player.isMovingLeft && player.getPositionX() > 0){
        movement.x -= player.getSpeed();
    }
    if(player.isMovingRight && player.getPositionX() < window_width - (player.getPlayerDot().getRadius()) * 2){
        movement.x += player.getSpeed();
    }
    if(player.isRotatingRight){
        rotation += player.getRotationSpeed();
    }
    if(player.isRotatingLeft){
        rotation -= player.getRotationSpeed();
    }

    player.move(movement * deltaTime.asSeconds());
    player.rotate(rotation * deltaTime.asSeconds());


}

// Renderer
void Game::render() {
    m_window.clear();

    // Rysuj Hud
        // Pasek Życia
        // Ilość amunicji
        // Jaka broń

    // Rysuj Minimapę
        // Rysuj Teren
        // Rysuj Gracza
        m_window.draw(player.getPlayerDot());
        m_window.draw(player.getDirectionLine());


    //Rysuj Przeciwników

    //Wyświetl
    m_window.display();
}

// Cleanup
void Game::cleanup() {

}

// Sprawdź czy gra jest on czy off
bool Game::isRunning() const {
    return m_is_running;
}
