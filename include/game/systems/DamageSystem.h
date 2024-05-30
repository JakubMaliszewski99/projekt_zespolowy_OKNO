#pragma once

#include "../../core/ecs/System.h"
#include "../../core/engine/InputManager.h"
#include "../components/TransformComponent.h"
#include "../components/WeaponComponent.h"
#include "../components/CollectableComponent.h"
#include "../components/HealthComponent.h"
#include "../components/DamageComponent.h"
#include "../../core/math/utilities.h"
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <iostream>

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
                this->projectileFire(transform, weapon);
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

            if((angleDiff < WeaponComponent::meeleSpread_angle) && (distance < WeaponComponent::meeleRange)) {
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

        std::vector<std::pair<Entity, float>> entitiesToBeHit_pairedWithDistance;
        
        for (auto const &entity : m_entities) {
            auto &health = m_manager->getComponent<HealthComponent>(entity);
            auto &targetTransform = m_manager->getComponent<TransformComponent>(entity);
            
            auto angleToEntity = angleBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, targetTransform.positionX, targetTransform.positionY);
            auto angleDiff = abs(angleToEntity - shooterTransform.angle);

            if(angleDiff > 2 * M_PI)
                angleDiff -= 2 * M_PI;

            if((abs(distanceFromLineOfHitscan(shooterTransform, targetTransform)) < WeaponComponent::weaponSpread_distance) && 
                                                                            (angleDiff < WeaponComponent::weaponSpread_angle)) {
                if(entity == m_playerEntity)
                    continue;

                auto distance = distanceBetweenPoints(shooterTransform.positionX, shooterTransform.positionY, 
                                                        targetTransform.positionX, targetTransform.positionY);
                entitiesToBeHit_pairedWithDistance.push_back(std::make_pair(entity, distance));
            }
        }
        
        if(!entitiesToBeHit_pairedWithDistance.empty()) {
            auto closestPair = entitiesToBeHit_pairedWithDistance.front();
            for (auto entity_pair : entitiesToBeHit_pairedWithDistance) {
                if(entity_pair.second < closestPair.second) {
                    closestPair = entity_pair;
                }
            }
            
            hitEntity(closestPair.first);
        }
    }

    void projectileFire(TransformComponent& shooterTransform, WeaponComponent& weapon) {
        Entity projectile = m_manager->createEntity();
        m_manager->addComponent(
            projectile,
            TransformComponent{shooterTransform.positionX, shooterTransform.positionY,
                         shooterTransform.positionZ, shooterTransform.targetPositonZ,
                         velocityScalarToVector(WeaponComponent::projectileSpeed, shooterTransform.angle),
                         shooterTransform.angle});
        m_manager->addComponent(projectile, DamageComponent{weapon.damage, WeaponComponent::projectileExplosionRange});
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