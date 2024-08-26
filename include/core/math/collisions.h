#pragma once
#include "utilities.h"
#include <optional>

// TODO: Move it to some class?
bool pointCircleCollision(float px, float py, float cx, float cy, float r);
bool linePointCollision(float x1, float y1, float x2, float y2, float px,
                        float py);
bool lineCircleCollision(float x1, float y1, float x2, float y2, float cx,
                         float cy, float r, sf::Vector2f &hit);
std::optional<sf::Vector2f> segmentSegmentCollision(float x1, float y1,
                                                    float x2, float y2,
                                                    float x3, float y3,
                                                    float x4, float y4);
std::optional<sf::Vector2f>
lineSegmentCollision(float segmentX1, float segmentY1, float segmentX2,
                     float segmentY2, float lineStartX, float lineStartY,
                     float lineAngle);
