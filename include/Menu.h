#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button(sf::Vector2f position, sf::Vector2f size, std::string text);

    bool contains(sf::Vector2f point);
    void draw(sf::RenderWindow& window);
    void setOnClick(std::function<void()> onClick);
    std::function<void()> onClick;

private:
    sf::RectangleShape rect;
    sf::Text buttonText;
    sf::Font font;

};

class Menu {
public:
    Menu();
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Button> buttons;
};

#endif // MENU_H