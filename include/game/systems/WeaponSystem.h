#pragma once

#include "../../core/ecs/System.h"
#include "../components/WeaponComponent.h"
#include "../../core/engine/InputManager.h"

#include <iostream>

class WeaponSystem : public System{
public:
    void init(std::shared_ptr<ECSManager> manager) { m_manager = manager; }

    void update(float dt)
    {
        for (auto const &entity : m_entities) {
            auto &controllable =
                m_manager->getComponent<ControllableComponent>(entity);

            if (!controllable.isPlayer) {
                continue;
            }

            auto &weapon = m_manager->getComponent<WeaponComponent>(entity);

            weapon.timeFromLastShot += dt;
            if(weapon.timeFromLastShot > 1/weapon.fireRate)
                weapon.readyToFire = true;

            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num1)) {
                weapon.activeWeaponType = WeaponType::eHandWeapon;
                weapon.readyToFire = true;
                weapon.fired = false;
            }
            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num2)) {
                weapon.activeWeaponType = WeaponType::eRangeWeapon;
                weapon.readyToFire = true;
                weapon.fired = false;
            }
            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num3)) {
                weapon.activeWeaponType = WeaponType::eLauncher;
                weapon.readyToFire = true;
                weapon.fired = false;                
            }
            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Space)) {
                if(weapon.readyToFire == true){
                    weapon.fired = true;
                    weapon.readyToFire = false;
                    weapon.timeFromLastShot = 0;
                }
            }
            //TODO test purposes only
            //std::cout << weapon.equipedWeapons[weapon.activeWeaponType] << std::endl;
        }
    }
private:
    
    std::shared_ptr<ECSManager> m_manager;

};