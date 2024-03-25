#include "../include/Painter2D.h"

Painter2D::Painter2D()
{
    playerRadius = 5.0f;
    enemyRadius = 10.0f;
    playerColor = sf::Color::White;
    enemyColor = sf::Color::Red;

    // Kropka oznaczaj¥ca playera
    playerDot.setRadius(playerRadius);
    playerDot.setFillColor(playerColor);

    //Kropka przeciwnika
    enemyDot.setRadius(enemyRadius);
    enemyDot.setFillColor(enemyColor);

    // Linia wskazuj¥ca kierunek patrzenia
    directionLine.setPrimitiveType(sf::Lines);
    directionLine.resize(2);
    directionLine[0].color = sf::Color::Red;
    directionLine[1].color = sf::Color::Red;
}

Painter2D::~Painter2D()
{

}


void Painter2D::draw(Player* player, sf::RenderWindow* window)
{
    playerDot.setPosition(player->getPositionX(), player->getPositionY());
    window->draw(playerDot);

    float playerAngle = player->getAngle();
    direction = sf::Vector2f(std::cos(playerAngle), std::sin(playerAngle));
    directionLine[0].position = playerDot.getPosition() + sf::Vector2f(playerDot.getRadius(), playerDot.getRadius());
    directionLine[1].position = directionLine[0].position + direction * 18.f;

    window->draw(directionLine);
}

void Painter2D::draw(Enemy* enemy, sf::RenderWindow* window)
{
    enemyDot.setPosition(enemy->getPositionX(), enemy->getPositionY());
    window->draw(enemyDot);
}