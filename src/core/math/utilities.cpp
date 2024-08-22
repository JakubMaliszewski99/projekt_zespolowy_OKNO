#include "../../../include/core/math/utilities.h"
#include "../../../include/core/core.h"
#include <cmath>
#include <iostream>

int angle2x(float angle) {
  int x;
  if (angle > 0) {
    x = SCREEN_DIST - tan(DEG2RAD(angle)) * H_WIDTH;
  } else {
    x = -tan(DEG2RAD(angle)) * H_WIDTH + SCREEN_DIST;
  }
  return x;
}

float pointToAngle(sf::Vector2f player, sf::Vector2f vertex) {
  auto delta = vertex - player;
  return RAD2DEG(atan2f(delta.y, delta.x));
}

float normalizeAngle(float angle) {
  angle = fmod(angle, 360);
  if (angle < 0) {
    angle += 360;
  }
  return angle;
}

float normalizeRadianAngle(float angle) {
  float twoPi = 2 * M_PI;
  return angle - twoPi * floor(angle / twoPi);
}

sf::Vector2f convertMapVertexToVector(mapvertexes_t vertex) {
  return sf::Vector2f(vertex.x, vertex.y);
}

bool checkBoundingBox(float playerX, float playerY, float playerAngle,
                      int16_t top, int16_t bottom, int16_t left,
                      int16_t right) {
  sf::Vector2f a = sf::Vector2f(left, bottom);
  sf::Vector2f b = sf::Vector2f(left, top);
  sf::Vector2f c = sf::Vector2f(right, top);
  sf::Vector2f d = sf::Vector2f(right, bottom);

  std::pair<sf::Vector2f, sf::Vector2f> bboxSides[2];
  int pairSidesCount = 0;
  if (playerX < left) {
    if (playerY > top) {
      bboxSides[0] = std::make_pair(b, a);
      bboxSides[1] = std::make_pair(c, b);
      pairSidesCount = 2;
    } else if (playerY < bottom) {
      bboxSides[0] = std::make_pair(b, a);
      bboxSides[1] = std::make_pair(a, d);
      pairSidesCount = 2;
    } else {
      bboxSides[0] = std::make_pair(b, a);
      pairSidesCount = 1;
    }
  } else if (playerX > right) {
    if (playerY > top) {
      bboxSides[0] = std::make_pair(c, b);
      bboxSides[1] = std::make_pair(d, c);
      pairSidesCount = 2;
    } else if (playerY < bottom) {
      bboxSides[0] = std::make_pair(a, d);
      bboxSides[1] = std::make_pair(d, c);
      pairSidesCount = 2;
    } else {
      bboxSides[0] = std::make_pair(d, c);
      pairSidesCount = 1;
    }
  } else {
    if (playerY > top) {
      bboxSides[0] = std::make_pair(c, b);
      pairSidesCount = 1;
    } else if (playerY < bottom) {
      bboxSides[0] = std::make_pair(a, d);
      pairSidesCount = 1;
    } else {
      return true;
    }
  }

  for (int i = 0; i < pairSidesCount; i++) {
    auto sides = bboxSides[i];
    float angle1 = pointToAngle(sf::Vector2f(playerX, playerY), sides.first);
    float angle2 = pointToAngle(sf::Vector2f(playerX, playerY), sides.second);
    float span = normalizeAngle(angle1 - angle2);

    angle1 -= playerAngle;
    float span1 = normalizeAngle(angle1 + H_FOV);
    if (span1 > FOV) {
      if (span1 >= span + FOV) {
        continue;
      }
    }
    return true;
  }

  return false;
}

std::tuple<bool, float, float, float>
backfaceCullingSegment(float playerX, float playerY, float playerAngle,
                       sf::Vector2f v1, sf::Vector2f v2) {
  float angle1 = pointToAngle(sf::Vector2f(playerX, playerY), v1);
  float angle2 = pointToAngle(sf::Vector2f(playerX, playerY), v2);

  float span = normalizeAngle(angle1 - angle2);

  if (span >= 180.0f) {
    return std::make_tuple(false, 0, 0, 0);
  }

  float origAngle1 = angle1;

  angle1 -= playerAngle;
  angle2 -= playerAngle;

  float span1 = normalizeAngle(angle1 + H_FOV);
  if (span1 > FOV) {
    if (span1 >= span + FOV) {
      return std::make_tuple(false, 0, 0, 0);
    }
    angle1 = H_FOV;
  }

  float span2 = normalizeAngle(H_FOV - angle2);
  if (span2 > FOV) {
    if (span2 >= span + FOV) {
      return std::make_tuple(false, 0, 0, 0);
    }
    angle2 = -H_FOV;
  }

  return std::make_tuple(true, angle1, angle2, origAngle1);
}

float xToAngle(float x) { 
  float angle = RAD2DEG(atan((H_WIDTH - x) / SCREEN_DIST));
  return angle;
}

float scaleFromGlobalAngle(float x, float rwNormalAngle, float rwDistance, float playerAngle) {
  float xAngle = xToAngle(x);
  float num = SCREEN_DIST * cosf(DEG2RAD(rwNormalAngle - xAngle - playerAngle));
  float den = rwDistance * cosf(DEG2RAD(xAngle));

  float scale = num / den;
  scale = std::min<float>(64.0f, std::max<float>(0.00390625, scale));

  return scale;
}

float distance(float x1, float y1, float x2, float y2) {
  float dx = x1 - x2;
  float dy = y1 - y2;

  float dist = sqrtf(dx * dx + dy * dy);

  return dist; 
}

float vectorLength(float x, float y) { return sqrtf(x * x + y * y); }

float vectorLength(sf::Vector2f vec) { 
  return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f normalize(sf::Vector2f vec) { 
  float length = vectorLength(vec);
  sf::Vector2f normalized = vec / length;
  return normalized;
}

sf::Vector2f lineNormal(float x1, float y1, float x2, float y2) {
  float dx = x2 - x1;
  float dy = y2 - y1;

  //TODO: Take angle in account
  return sf::Vector2f(-dy, dx);
}

sf::Vector2f projectVectorOntoLine(sf::Vector2f vector, sf::Vector2f line) {
  float dotProduct = vector.x * line.x + vector.y * line.y;
  float lineLength = vectorLength(line);
  float newLength = dotProduct / (lineLength * lineLength);

  return newLength * line;
}

int negMod(int a, int b)
{
    return (b + (a % b)) % b;
}

sf::Vector2f velocityScalarToVector(float velocity, float angle_rad) {
  float x = cosf(angle_rad) * velocity;
  float y = sinf(angle_rad) * velocity;
  return sf::Vector2f(x, y);
}

sf::Vector2f segmentEnd(float startX, float startY, float angle, float length){
    float endX = startX + cosf(angle) * length;
    float endY = startY + sinf(angle) * length;
    return sf::Vector2f(endX, endY);
}

sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t) {
  return start + t * (end - start);
}

float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}