#pragma once
#include "../sprites/PlayerMinimapSprite.h"
#include <SFML/Graphics.hpp>

// TODO: Use safe pointers
// TODO: Change to shared_ptr?
struct MinimapSpriteComponent {
  sf::View camera;
  MinimapSprite *sprite;
  bool isPlayer;
};
