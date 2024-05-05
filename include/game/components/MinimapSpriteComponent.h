#pragma once
#include <SFML/Graphics.hpp>
#include "../sprites/PlayerMinimapSprite.h"

// TODO: Use safe pointers
// TODO: Change to shared_ptr?
struct MinimapSpriteComponent {
  sf::View camera;
  MinimapSprite* sprite;
  bool isPlayer;
};