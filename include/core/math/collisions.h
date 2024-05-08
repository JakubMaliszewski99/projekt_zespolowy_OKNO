#pragma once
#include "utilities.h"

// TODO: Move it to some class?
bool pointCircleCollision(float px, float py, float cx, float cy, float r);
bool linePointCollision(float x1, float y1, float x2, float y2, float px, float py);
bool lineCircleCollision(float x1, float y1, float x2, float y2, float cx,
                         float cy, float r, sf::Vector2f &hit);