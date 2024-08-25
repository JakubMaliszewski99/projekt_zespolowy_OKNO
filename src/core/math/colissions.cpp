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
                         float cy, float r, sf::Vector2f &hit) {
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

std::optional<sf::Vector2f>
segmentSegmentCollision(float x1, float y1, float x2, float y2, float x3,
                        float y3, float x4,
                        float y4) { // 1, 2 - first segm; 3, 4 - second segm;
  float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
  float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

  // if uA and uB are between 0-1, segments are colliding
  if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
    float intersectionX = x1 + (uA * (x2 - x1));
    float intersectionY = y1 + (uA * (y2 - y1));
    return sf::Vector2f(intersectionX, intersectionY);
  }

  return std::nullopt;
}

std::optional<sf::Vector2f>
lineSegmentCollision(float segmentX1, float segmentY1, float segmentX2,
                     float segmentY2, float lineStartX, float lineStartY,
                     float lineAngle) {
  float temp_distance = distance(lineStartX, lineStartY, segmentX1, segmentY1);
  float safe_distance =
      temp_distance *
      2; // safe meaning segment is for sure within this distance

  sf::Vector2f lineEnd =
      segmentEnd(lineStartX, lineStartY, lineAngle, safe_distance);

  return segmentSegmentCollision(lineStartX, lineStartY, lineEnd.x, lineEnd.y,
                                 segmentX1, segmentY1, segmentX2, segmentY2);
}
