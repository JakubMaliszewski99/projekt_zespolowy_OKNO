#include "../include/core/math/collisions.h"
#include <cmath>

// Kudos to Jeff Thompson: https://www.jeffreythompson.org/collision-detection/
bool pointCircleCollision(float px, float py, float cx, float cy, float r) {
  float dist = distance(px, py, cx, cy);

  if (dist <= r) {
    return true;
  }
  return false;
}

bool linePointCollision(float x1, float y1, float x2, float y2, float px,
                        float py) {
  float lineLen = distance(x1, y1, x2, y2);

  float d1 = distance(px, py, x1, y1);
  float d2 = distance(px, py, x2, y2);

  float buffer = 0.1;

  if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
    return true;
  }
  return false;
}

bool lineCircleCollision(float x1, float y1, float x2, float y2, float cx,
                         float cy, float r, sf::Vector2f& hit) {
  bool inside1 = pointCircleCollision(x1, y1, cx, cy, r);
  bool inside2 = pointCircleCollision(x2, y2, cx, cy, r);

  if (inside1 || inside2) {
    return true;
  }

  float len = distance(x1, y1, x2, y2);
  float dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2);

  float closestX = x1 + (dot * (x2 - x1));
  float closestY = y1 + (dot * (y2 - y1));

  bool onSegment = linePointCollision(x1, y1, x2, y2, closestX, closestY);
  if (!onSegment) {
    return false;
  }

  float distX = closestX - cx;
  float distY = closestY - cy;
  float distance = sqrt((distX * distX) + (distY * distY));

  hit = sf::Vector2f(distX, distY);

  if (distance <= r) {
    return true;
  }
  return false;
}
