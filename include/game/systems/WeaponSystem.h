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
            auto &transform = m_manager->getComponent<TransformComponent>(entity);
            auto &controllable =
                m_manager->getComponent<ControllableComponent>(entity);
            auto &drawable = m_manager->getComponent<MinimapSpriteComponent>(entity);

            if (!controllable.isPlayer) {
                continue;
            }

            auto &weapon = m_manager->getComponent<WeaponComponent>(entity);

            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num1)) {
                weapon.equipedWeaponType = WeaponType::eHandWeapon;
            }
            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num2)) {
                weapon.equipedWeaponType = WeaponType::eRangeWeapon;
            }
            if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Num3)) {
                weapon.equipedWeaponType = WeaponType::eLauncher;
            }
        }
    }
private:
    
    std::shared_ptr<ECSManager> m_manager;

};