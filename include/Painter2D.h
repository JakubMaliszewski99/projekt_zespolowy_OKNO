#ifndef PAINTER2D_H
#define PAINTER2D_H

#pragma once

#include "../include/Player.h"
#include "../include/Enemy.h"
#include <SFML/Graphics.hpp>


class Painter2D
{
    public:
        Painter2D();
        ~Painter2D();

        void draw(Player* player, sf::RenderWindow* window);
        void draw(Enemy* enemy, sf::RenderWindow* window);
    private:

        // Rysowanie kropki gracza na minimapce
        float playerRadius;
        sf::Color playerColor;
        sf::CircleShape playerDot;

        // Rysowanie przeciwnika na minimapce
        float enemyRadius;
        sf::Color enemyColor;
        sf::CircleShape enemyDot;

        // Rysowanie linii gracza wyznaczaj¥cej zwrot gracza
        sf::Vector2f direction;
        sf::VertexArray directionLine;
};

#endif