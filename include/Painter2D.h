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
        void draw(Enemy* enemy);
    private:

        // Rysowanie kropki gracza na minimapce
        float radius;
        sf::Color playerColor;
        sf::CircleShape playerDot;

        // Rysowanie linii gracza wyznaczaj¥cej zwrot gracza
        sf::Vector2f direction;
        sf::VertexArray directionLine;
};

#endif