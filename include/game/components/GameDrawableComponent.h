#pragma once
#include "../drawable/GameDrawable.h"

enum GameDrawableType { eCollectable, eMonster, eLevel };

// TODO: Change to shared_ptr?
struct GameDrawableComponent {
  GameDrawableType type;
  GameDrawable *drawable;
};
