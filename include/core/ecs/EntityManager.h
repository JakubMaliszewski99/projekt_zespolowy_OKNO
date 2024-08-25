#pragma once
#include "ComponentBase.h"
#include "Entity.h"
#include <array>
#include <queue>

class EntityManager {
public:
  EntityManager();
  Entity createEntity();
  void destroyEntity(Entity entity);
  void setSignature(Entity entity, Signature signature);
  Signature getSignature(Entity entity);

private:
  std::queue<Entity> m_availableEntities{};
  std::array<Signature, MAX_ENTITIES> m_signatures{};
  uint32_t m_livingEntityCount{};
};
