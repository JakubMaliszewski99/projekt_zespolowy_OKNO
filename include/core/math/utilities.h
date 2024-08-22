#pragma once
#include <SFML/Graphics.hpp>
#include "../engine/GameLevel.h"

int angle2x(float angle);
float pointToAngle(sf::Vector2f player, sf::Vector2f vertex);
float normalizeAngle(float angle);
float normalizeRadianAngle(float angle);

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
float distance(float x1, float y1, float x2, float y2);
float vectorLength(float x, float y);
float vectorLength(sf::Vector2f vec);
sf::Vector2f normalize(sf::Vector2f vec);
sf::Vector2f lineNormal(float x1, float y1, float x2, float y2);
sf::Vector2f projectVectorOntoLine(sf::Vector2f vector, sf::Vector2f line);
sf::Vector2f velocityScalarToVector(float velocity, float angle);
sf::Vector2f segmentEnd(float startX, float startY, float angle, float length);
sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
float dot(const sf::Vector2f& a, const sf::Vector2f& b);
int negMod(int a, int b);