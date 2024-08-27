#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

#define M_PI 3.14159265358979323846
#define UNUSED(x) (void)(x)

//fov rays constants
const float FOV = 90; //don't touch this (rendering depends on this)
const float H_FOV = FOV / 2;
const float FOV_ENEMIES = 60;
const float FOV_LENGTH = 700.0f;


#define WIDTH 1600
#define HEIGHT 1000
#define H_WIDTH WIDTH / 2
#define H_HEIGHT HEIGHT / 2
#define PLAYER_HEIGHT 41

#define DEG2RAD(x) ((x) * M_PI / 180)
#define RAD2DEG(x) ((x) * 180 / M_PI)

const float SCREEN_DIST = H_WIDTH / tanf(DEG2RAD(H_FOV));

struct DebugSettings {
  bool displayFov = false;
  bool displayVisibleNodes = false;
  bool displayVisibleSegments = false;
};

// TODO: Move somewhere else??
sf::Color getRandomColor();
