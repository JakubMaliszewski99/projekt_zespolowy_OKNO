#pragma once

#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"
#include "../components/TransformComponent.h"
#include "../components/HealthComponent.h"
#include <math.h>
#include <vector>
#include <utility>
#include <iostream>

class DamageSystem : public System{
public:
    void init(std::shared_ptr<ECSManager> manager) { m_manager = manager; }

    void update(float dt)
    {
        for (auto const &entity : m_entities) {
            auto &weapon =
                m_manager->getComponent<WeaponComponent>(entity);
            auto &transform = 
                m_manager->getComponent<TransformComponent>(entity);

            if(weapon.fired == true) {
                weapon.fired = false;
                if(weapon.activeWeaponType == WeaponType::eRangeWeapon) {
                    this->hitscanFire(transform);
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

    void hitscanFire(TransformComponent& shooterTransform){
        //TODO below code is ignoring walls:
        std::vector<std::pair<Entity, float>> entitiesToBeHit;

        for (auto const &entity : m_entities) {
            auto &healthComp =
                m_manager->getComponent<HealthComponent>(entity);
            auto &transform = 
                m_manager->getComponent<TransformComponent>(entity);
            
            //TODO check if entity would be hit
            //TODO if so, add it to vector
        }
        //TODO find closest entity from vector
        //TODO decrement health
    }

    void projectileFire() {
        //TODO - projectile launching
    }

    float distanceFromLineOfHitscan(TransformComponent& shooterTransform, TransformComponent& targetTransform){

    }

    float angleBetweenPoints(float x1, float y1, float x2, float y2){
        return 0;
    }

    float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
        float x = x1 - x2;
        float y = y1 - y2;
        float distance = sqrtf(x * x + y * y);

        return distance;
    }

    std::shared_ptr<ECSManager> m_manager;

};