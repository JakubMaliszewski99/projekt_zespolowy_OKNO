#include "../include/core/core.h"

//Generowanie randomowego koloru dla poszczeg¢lnych sektor¢w
sf::Color getRandomColor() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(50, 255);

  sf::Color color;
  color.r = distr(gen);
  color.g = distr(gen);
  color.b = distr(gen);

  return color;
}