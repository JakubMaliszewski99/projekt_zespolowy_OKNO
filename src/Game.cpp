#include "../include/Game.h"

// Konstruktor
Game::Game(){
    std::clog << "Utworzenie obiektu game" << std::endl;

    m_is_running = true;
    window_width = WINDOW_X_SIZE;
    window_height = WINDOW_Y_SIZE;
    frames_per_second = 60;

    enemy = Enemy(window_width, window_height);
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
            std::clog << "W - Przód" << std::endl;
            player.isMovingForwards = isPressed;
            break;
        case sf::Keyboard::S:
            std::clog << "S - Tył" << std::endl;
            player.isMovingBackwards = isPressed;
            break;
        case sf::Keyboard::A:
            std::clog << "A - Lewo" << std::endl;
            player.isMovingLeft = isPressed;
            break;
        case sf::Keyboard::D:
            std::clog << "D - Prawo" << std::endl;
            player.isMovingRight = isPressed;
            break;
        // Rotacja
        case sf::Keyboard::E:
            std::clog << "E - Obrót prawo" << std::endl;
            player.isRotatingRight = isPressed;
            break;
        case sf::Keyboard::Q:
            std::clog << "Q - Obrót lewo" << std::endl;
            player.isRotatingLeft = isPressed;
            break;
        // Bieg
        case sf::Keyboard::LShift:
            std::clog << "Shift - Bieg" << std::endl;
            player.isSprinting = isPressed;
            break;
        // Strzelanie
        // Interakcja
    }
}
// Logika gry
void Game::update(sf::Time deltaTime) {

    sf::Vector2f movement(0.f, 0.f);
    float velocity = player.getSpeed() * deltaTime.asSeconds();
    float rotation = 0.f;
    player.isSprinting? player.setSpeed(PLAYER_DEFAULT_SPEED * 2) : player.setSpeed(PLAYER_DEFAULT_SPEED);
    if(player.isMovingForwards){
        movement.x += cos(player.getAngle()) * velocity;
        movement.y += sin(player.getAngle()) * velocity;
    }
    if(player.isMovingBackwards){
        movement.x -= cos(player.getAngle()) * velocity;
        movement.y -= sin(player.getAngle()) * velocity;
    }
    if(player.isMovingLeft){
        movement.x -= cos(player.getAngle() + 90 * (M_PI /180)) * velocity;
        movement.y -= sin(player.getAngle() + 90 * (M_PI /180)) * velocity;

    }
    if(player.isMovingRight){
        movement.x += cos(player.getAngle() + 90 * (M_PI /180)) * velocity;
        movement.y += sin(player.getAngle() + 90 * (M_PI /180)) * velocity;
    }
    if(player.isRotatingRight){
        rotation += player.getRotationSpeed() * deltaTime.asSeconds();
    }
    if(player.isRotatingLeft){
        rotation -= player.getRotationSpeed() * deltaTime.asSeconds();
    }

    player.move(movement);
    player.rotate(rotation);

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
        //m_window.draw(player.getPlayerDot());
        //m_window.draw(player.getDirectionLine());
        renderer.draw(&player, &m_window);
        renderer.draw(&enemy, &m_window);
    //Rysuj Przeciwników

    //R

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
