#include "core/ecs/EntityManager.h"

EntityManager::EntityManager() {
  for (Entity entity = 0; entity < MAX_ENTITIES; entity++) {
    m_availableEntities.push(entity);
  }
}

Entity EntityManager::createEntity() {
  if (m_livingEntityCount >= MAX_ENTITIES) {
    return -1;
  }

  Entity id = m_availableEntities.front();
  m_availableEntities.pop();
  m_livingEntityCount++;

  return id;
}

void EntityManager::destroyEntity(Entity entity) {
  m_signatures[entity].reset();
  m_availableEntities.push(entity);
  m_livingEntityCount--;
}

void EntityManager::setSignature(Entity entity, Signature signature) {
  m_signatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity) {
  return m_signatures[entity];
}
