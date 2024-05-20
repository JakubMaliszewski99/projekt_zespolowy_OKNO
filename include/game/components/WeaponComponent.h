#pragma once

#include "CollectableComponent.h"

enum WeaponType{
    eHandWeapon, eRangeWeapon, eLauncher
};

enum WeaponModel{
    eWeaponModelFist = 2001, eWeaponModelPistol = 2002, eWeaponModelBFG9000 = 2003, eWeaponModelChaingun = 2004, eWeaponModelChainsaw = 2005, eWeaponModelPlasmaGun = 2006,
    eWeaponModelRocketLauncher = 2007, eWeaponModelShotgun = 2008, eWeaponModelSuperShotgun = 2009, eWeaponModelNone = 2010
};

struct WeaponComponent{
    WeaponModel activeWeapon;
    WeaponType equipedWeaponType;
};