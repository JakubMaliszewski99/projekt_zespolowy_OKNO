#pragma once
#include <SFML/Graphics.hpp>
#include "../engine/GameLevel.h"

int angle2x(float angle);
float pointToAngle(sf::Vector2f player, sf::Vector2f vertex);
float normalizeAngle(float angle);

// TODO: Finally delete?
sf::Vector2f convertMapVertexToVector(mapvertexes_t vertex);

bool checkBoundingBox(float playerX, float playerY, float playerAngle,
                      int16_t top, int16_t bottom, int16_t left, int16_t right);
std::tuple<bool, float, float, float> backfaceCullingSegment(float playerX,
                                                             float playerY,
                                                             float playerAngle,
                                                             sf::Vector2f v1, sf::Vector2f v2);

float scaleFromGlobalAngle(float x, float rwNormalAngle, float rwDistance,
                           float playerAngle);