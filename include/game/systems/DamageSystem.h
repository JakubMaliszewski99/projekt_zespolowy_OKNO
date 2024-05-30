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
#include <algorithm>

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

        handleFiredProjectiles();
        
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
        m_manager->addComponent(projectile, DamageComponent{weapon.damage, 
                                                            WeaponComponent::projectileExplosionRange, 
                                                            WeaponComponent::projectileExplosionRadius});
        projectilesFired.push_back(projectile);
    }

    void handleFiredProjectiles(){
        for(auto &projectile: projectilesFired){
            auto &projectileTransform = m_manager->getComponent<TransformComponent>(projectile);
            auto &damageComponent = m_manager->getComponent<DamageComponent>(projectile);

            moveProjectile(projectileTransform);
            
            for (auto &entity : m_entities){
                
                if(entity == m_playerEntity)
                    continue;

                auto &targetTransform = m_manager->getComponent<TransformComponent>(entity);
                if(distanceBetweenPoints(projectileTransform.positionX, projectileTransform.positionY,
                                            targetTransform.positionX, targetTransform.positionY) < damageComponent.activationRange){
                    projectileExplode(projectile, projectileTransform, damageComponent);
                    break;
                }
                else if (projectileHitWall(projectileTransform))
                {
                    projectileExplode(projectile, projectileTransform, damageComponent);
                }
            }            
        }
    }

    bool projectileHitWall(TransformComponent& transform) {
        return false;
    }

    void projectileExplode(Entity projectile, TransformComponent& transform, DamageComponent& damageComponent){
        std::vector<Entity> entitiesToBeAffected;           

        for (auto &entity : m_entities){
                if(entity == m_playerEntity)
                    continue;

                auto &targetTransform = m_manager->getComponent<TransformComponent>(entity);
                if(distanceBetweenPoints(transform.positionX, transform.positionY,
                                            targetTransform.positionX, targetTransform.positionY) < damageComponent.explosionRadius){
                    entitiesToBeAffected.push_back(entity);
                }
        }
        
        for (auto &entity: entitiesToBeAffected){
            // TODO add damage info:
            hitEntity(entity);
        }

        auto iterator = std::find(projectilesFired.begin(), projectilesFired.end(), projectile);
        projectilesFired.erase(iterator);
        m_manager->destroyEntity(projectile);
    }

    // TODO to be moved to other system:
    void moveProjectile(TransformComponent& transform) {
        sf::Vector2f velocity = transform.velocity;
        transform.velocity = normalize(velocity);
        transform.positionX += velocity.x;
        transform.positionY += velocity.y;
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

    void hitEntity(Entity& entity/*TODO :, float damage*/){
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
    std::vector<Entity> projectilesFired;
};