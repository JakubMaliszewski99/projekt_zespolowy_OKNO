#pragma once

#include "CollectableComponent.h"

enum WeaponType{
    handWeapon, rangeWeapon, launcher
};

struct WeaponComponent{
    WeaponType equipedWeaponType;
};