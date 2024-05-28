#pragma once

#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"
#include "../components/TransformComponent.h"
#include "../components/WeaponComponent.h"
#include "../components/CollectableComponent.h"
#include "../components/HealthComponent.h"
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <iostream>

//TODO to be moved to weapon system/component:
const float weaponSpread_inDegrees = 10;
const float weaponSpread_angle = 2*M_PI/360 * weaponSpread_inDegrees;
const float weaponSpread_distance = 100;
const float meeleRange = 100;

class DamageSystem : public System{
public:
    void init(std::shared_ptr<ECSManager> manager, Entity playerEntity) {
        m_manager = manager;
        m_playerEntity = playerEntity;
    }

    void update(float dt)
    {
        auto &transform = m_manager->getComponent<TransformComponent>(m_playerEntity);
        auto &weapon = m_manager->getComponent<WeaponComponent>(m_playerEntity);

        if(weapon.fired == true) {
            weapon.fired = false;
            if(weapon.activeWeaponType == WeaponType::eRangeWeapon) {
                this->hitscanFire(transform);
            }
            else if (weapon.activeWeaponType == WeaponType::eHandWeapon) {
                this->meeleFire(transform);
            }
            else {
                this->projectileFire();
            }
        }
    }

private:
    void meeleFire(TransformComponent& shooterTransform){
        std::vector<std::pair<Entity, float>> entitiesToBeHit;
        
        for (auto const &entity : m_entities) {
            auto &health = m_manager->getComponent<HealthComponent>(entity);
            auto &targetTransform = m_manager->getComponent<TransformComponent>(entity);
            
            auto angleToEntity = angleBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, targetTransform.positionX, targetTransform.positionY);
            auto angleDiff = abs(angleToEntity - shooterTransform.angle);
            if(angleDiff > 2 * M_PI)
                angleDiff -= 2 * M_PI;

            auto distance = distanceBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, 
                                                        targetTransform.positionX, targetTransform.positionY);

            //TODO replace M_PI/2 with variable:
            if((angleDiff < M_PI/2) && (distance < meeleRange)) {
                if(entity == m_playerEntity)
                    continue;
                entitiesToBeHit.push_back(std::make_pair(entity, distance));
            }
        }
        
        if(!entitiesToBeHit.empty()) {
            auto closestEntity = entitiesToBeHit.front();
            for (auto entity_pair : entitiesToBeHit) {
                if(entity_pair.second < closestEntity.second) {
                    closestEntity = entity_pair;
                }
            }
            
            hitEntity(closestEntity.first);
        }
    }

    void hitscanFire(TransformComponent& shooterTransform){
        //TODO below code is ignoring walls:

        std::vector<std::pair<Entity, float>> entitiesToBeHit;
        
        for (auto const &entity : m_entities) {
            auto &health = m_manager->getComponent<HealthComponent>(entity);
            auto &targetTransform = m_manager->getComponent<TransformComponent>(entity);
            
            auto angleToEntity = angleBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, targetTransform.positionX, targetTransform.positionY);
            auto angleDiff = abs(angleToEntity - shooterTransform.angle);
            if(angleDiff > 2 * M_PI)
                angleDiff -= 2 * M_PI;

            //TODO replace M_PI/2 with variable:
            if((abs(distanceFromLineOfHitscan(shooterTransform, targetTransform)) < weaponSpread_distance) && (angleDiff < M_PI/2)) {
                if(entity == m_playerEntity)
                    continue;

                auto distance = distanceBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, 
                                                        targetTransform.positionX, targetTransform.positionY);
                entitiesToBeHit.push_back(std::make_pair(entity, distance));
            }
        }
        
        if(!entitiesToBeHit.empty()) {
            auto closestEntity = entitiesToBeHit.front();
            for (auto entity_pair : entitiesToBeHit) {
                if(entity_pair.second < closestEntity.second) {
                    closestEntity = entity_pair;
                }
            }
            
            hitEntity(closestEntity.first);
        }
    }

    void projectileFire() {
        //TODO - projectile launching
    }

    float distanceFromLineOfHitscan(TransformComponent& shooterTransform, TransformComponent& targetTransform){
        auto sin = sinf(angleBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, 
                                        targetTransform.positionX, targetTransform.positionY)
                            - shooterTransform.angle);
        auto distance = distanceBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, 
                                        targetTransform.positionX, targetTransform.positionY);
        auto result = sin * distance;
        return result;
    }

    void hitEntity(Entity& entity/*, float damage*/){
        m_manager->destroyEntity(entity);
    }

    float angleBetweenPoints(float x1, float y1, float x2, float y2){
        float result = atan2f(y2-y1, x2-x1);
        return result;
    }

    float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
        float x = x1 - x2;
        float y = y1 - y2;
        float distance = sqrtf(x * x + y * y);

        return distance;
    }

    std::shared_ptr<ECSManager> m_manager;
    Entity m_playerEntity;
};