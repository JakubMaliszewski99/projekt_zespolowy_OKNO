#include "..\include\Menu.h"
#include <iostream>

Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text) {
    rect.setPosition(position);
    rect.setSize(size);
    rect.setFillColor(sf::Color::Blue);

    font.loadFromFile("..\\data\\font\\amazdoom\\AmazDooMLeft.ttf");
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(position.x + 10, position.y + 10);
}

bool Button::contains(sf::Vector2f point) {
    return rect.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(buttonText);
}

void Button::setOnClick(std::function<void()> onClick) {
    this->onClick = onClick;
}

Menu::Menu() {
    buttons.emplace_back(sf::Vector2f(100, 100), sf::Vector2f(200, 50), "Start Game");
    buttons.emplace_back(sf::Vector2f(100, 200), sf::Vector2f(200, 50), "Exit Game");

    buttons[0].setOnClick([this]() {
        std::cout << "Start Game clicked!" << std::endl;
        // Handle Start Game click here
    });

    buttons[1].setOnClick([this]() {
        std::cout << "Exit Game clicked!" << std::endl;
        // Handle Exit Game click here
    });
}

void Menu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        for (auto& button : buttons) {
            if (button.contains(mousePosition)) {
                if (button.onClick) {
                    button.onClick();
                }
            }
        }
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button.draw(window);
    }
}