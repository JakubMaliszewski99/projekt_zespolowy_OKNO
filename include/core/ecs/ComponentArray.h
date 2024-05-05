#pragma once
#include "Entity.h"
#include <cassert>

class IComponentArray {
public:
  virtual ~IComponentArray() = default;
  virtual void entityDestroyed(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
public:
  void insertData(Entity entity, T component) {
    assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() &&
           "Component added to same entity more than once.");

    // Put new entry at end and update the maps
    size_t newIndex = m_size;
    m_entityToIndexMap[entity] = newIndex;
    m_indexToEntityMap[newIndex] = entity;
    m_componentArray[newIndex] = component;
    ++m_size;
  }

  void removeData(Entity entity) {
    assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() &&
           "Removing non-existent component.");

    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
    size_t indexOfLastElement = m_size - 1;
    m_componentArray[indexOfRemovedEntity] =
        m_componentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
    m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    m_entityToIndexMap.erase(entity);
    m_indexToEntityMap.erase(indexOfLastElement);

    --m_size;
  }

  T &getData(Entity entity) {
    assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() &&
           "Retrieving non-existent component.");

    // Return a reference to the entity's component
    return m_componentArray[m_entityToIndexMap[entity]];
  }

  void entityDestroyed(Entity entity) override {
    if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()) {
      // Remove the entity's component if it existed
      removeData(entity);
    }
  }

private:
  std::array<T, MAX_ENTITIES> m_componentArray;
  std::unordered_map<Entity, size_t> m_entityToIndexMap;
  std::unordered_map<size_t, Entity> m_indexToEntityMap;
  size_t m_size;
};