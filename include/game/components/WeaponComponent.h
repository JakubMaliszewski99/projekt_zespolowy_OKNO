#pragma once

#include "../../core/core.h"
#include "CollectableComponent.h"

enum WeaponType{
    eHandWeapon, eRangeWeapon, eLauncher
};

enum WeaponModel{
    eWeaponModelFist = 2001, eWeaponModelPistol = 2002, eWeaponModelBFG9000 = 2003, eWeaponModelChaingun = 2004, eWeaponModelChainsaw = 2005, eWeaponModelPlasmaGun = 2006,
    eWeaponModelRocketLauncher = 2007, eWeaponModelShotgun = 2008, eWeaponModelSuperShotgun = 2009, eWeaponModelNone = 2010
};

struct WeaponComponent{
    //general info:
    static constexpr float weaponSpread_inDegrees = 10;
    static constexpr float weaponSpread_angle = 2*M_PI/360 * weaponSpread_inDegrees;
    static constexpr const float weaponSpread_distance = 100;
    static constexpr const float meeleRange = 100;
    static constexpr const float meeleSpread_inDegrees = 45;
    static constexpr const float meeleSpread_angle = 2*M_PI/360 * meeleSpread_inDegrees;
    static constexpr const float projectileSpeed = 100;
    static constexpr const float projectileExplosionRange = 100;

    // weapon info:
    WeaponModel equipedWeapons[3]; //0-hand; 1-range; 2-lanucher
    WeaponType activeWeaponType;
    
    // time management:
    bool fired;
    bool readyToFire;
    float timeFromLastShot;

    // weapon stats: 
    float fireRate; //per second
    float damage;
};