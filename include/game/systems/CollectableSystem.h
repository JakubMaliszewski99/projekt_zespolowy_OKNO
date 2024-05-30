#pragma once
#include "../components/CollectableComponent.h"
#include "../components/TransformComponent.h"
#include "../include/core/ecs/ECSManager.h"

class CollectableSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager, Entity playerEntity) {
    m_manager = manager;
    m_playerEntity = playerEntity;
  }

  void update(float dt) {

    auto &playerTransform =
        m_manager->getComponent<TransformComponent>(m_playerEntity);

    std::vector<Entity> entitiesToDelete;
    for (auto const &entity : m_entities) {
      auto &collectableTransform =
          m_manager->getComponent<TransformComponent>(entity);
      auto &collectable = m_manager->getComponent<CollectableComponent>(entity);

      if (this->distance(playerTransform.positionX, playerTransform.positionY,
                         collectableTransform.positionX,
                         collectableTransform.positionY) < collectable.radius) {
        // TODO: Add effect after collecting
        entitiesToDelete.push_back(entity);
      }

    }

    if (entitiesToDelete.empty()) {
      return;
    }

    for (auto entity : entitiesToDelete) {
      m_manager->destroyEntity(entity);
    }
    
  }

private:
  float distance(float x1, float y1, float x2, float y2) {
    float x = x1 - x2;
    float y = y1 - y2;
    float distance = sqrtf(x * x + y * y);

    return distance;
  }
  std::shared_ptr<ECSManager> m_manager;
  Entity m_playerEntity;
};