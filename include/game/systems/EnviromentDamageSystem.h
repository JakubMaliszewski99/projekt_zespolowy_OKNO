#pragma once
#include "../components/TransformComponent.h"
#include "../include/core/ecs/ECSManager.h"
#include <iostream>

class EnviromentDamageSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager) {
    m_manager = manager;
  }

  void update(float dt) {    
    for (auto const &entity : m_entities) {
        auto &damageComponent =
            m_manager->getComponent<DamageComponent>(entity);
        auto &transform =
            m_manager->getComponent<TransformComponent>(entity);
        
        std::cout << "damageComponent at: " << transform.positionX << " " << transform.positionY << std::endl;
        moveProjectile(transform);
    }
  }

private:
    // TODO to be moved to other system:
    void moveProjectile(TransformComponent& transform) {
        sf::Vector2f velocity = transform.velocity;
        transform.velocity = normalize(velocity);
        transform.positionX += velocity.x;
        transform.positionY += velocity.y;
    }

    std::shared_ptr<ECSManager> m_manager;
};