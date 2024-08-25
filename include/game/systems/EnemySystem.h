#pragma once
#include "../../core/ecs/System.h"
#include "../../core/math/collisions.h"
#include "../include/core/ecs/ECSManager.h"
#include "../components/EnemyComponent.h"
#include "../components/TransformComponent.h"

class EnemySystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager) { m_manager = manager; }

  void update(float dt, std::vector<Entity> enemyEntities){
    for (auto const &entity : enemyEntities){
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &enemy = m_manager->getComponent<EnemyComponent>(entity);

      // handle logic differently depending on the game state
      if(enemy.isAlive){
        switch(enemy.state){
          case Idle:
            break;
          case Patrol:
            break;
          case Chase:
            break;
          case Attack:
            break;
          default:
            break;
        }
      }
    }

  }

  bool isEnemy(int type) {
    switch (type) {
    case eImp:
    case eShotgunGuy:
    case eZombieMan:
      return true;
    default:
      return false;
    }
  }

private:
  std::shared_ptr<ECSManager> m_manager;
};
