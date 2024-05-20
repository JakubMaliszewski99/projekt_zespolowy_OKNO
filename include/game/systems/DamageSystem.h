#pragma once

#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"

#include <iostream>

class DamageSystem : public System{
public:
    void init(std::shared_ptr<ECSManager> manager) { m_manager = manager; }

    void update(float dt)
    {
        for (auto const &entity : m_entities) {
            auto &weapon =
                m_manager->getComponent<WeaponComponent>(entity);

            if(weapon.fired == true) {
                weapon.fired = false;
                if(weapon.activeWeaponType == WeaponType::eRangeWeapon) {
                    this->hitscanFire();
                }
                else if (weapon.activeWeaponType == WeaponType::eHandWeapon) {
                    this->meeleFire();
                }
                else {
                    this->projectileFire();
                }
            }
        }
    }

private:
    void meeleFire(){

    }

    void hitscanFire(){
        //TODO - hitscan...
    }

    void projectileFire() {
        //TODO - projectile launching
    }

    std::shared_ptr<ECSManager> m_manager;

};