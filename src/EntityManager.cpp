#include "EntityManager.h"
#include <algorithm>

void EntityManager::removeDeadEntities(EntityVec &vec) {

  auto deadIterator = std::remove_if(
      vec.begin(), vec.end(), [](const auto &e) { return !e->isAlive(); });
  vec.erase(deadIterator, vec.end());
}

// void EntityManager::init() {}
void EntityManager::update() {
  // TODO: add entities from m_ entitiesToAdd the proper locations
  //  -add them to the vector of all entties
  //  - add them to the vector inside the map, with the tag as a key
  for (auto &e : m_toAdd) {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }
  m_toAdd.clear();

  // remove dead entities from the vector of all entities
  removeDeadEntities(m_entities);

  // remove dead entites from each vector in the entity map
  // C++20 ay of iterating through [key, value] pairs in a map
  for (auto &[tag, entityVec] : m_entityMap) {
    removeDeadEntities(entityVec);
  }
}
std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
  m_totalEntities++;
  const size_t idNum = m_totalEntities;
  auto e = std::shared_ptr<Entity>(new Entity(idNum, tag));
  m_toAdd.push_back(e);

  // add to entity map implemented
  // m_entityMap[tag].push_back(e);

  return e;
}
const EntityVec &EntityManager::getEntities() const { return m_entities; }

const EntityVec &EntityManager::getEntities(const std::string &tag) {
  if (m_entityMap.find(tag) == m_entityMap.end()) {
    m_entityMap[tag] = EntityVec();
  }
  return m_entityMap[tag];
}
const EntityMap &EntityManager::getEntityMap() { return m_entityMap; }
const size_t &EntityManager::getTotalEntities() { return m_totalEntities; }
