#include "EntityManager.h"
#include <algorithm>

void EntityManager::removeDeadEntities(EntityVec &vec) {
  // TODO: remove "dead" entities from the entity vector being passed in
  // check entity alive variable
}

// void EntityManager::init() {}
void EntityManager::update() {
  // TODO: add entities from buffer vector toAdd to main vector m_entities
  // add entities to main vector
  // also add them to map vector

  // TODO: remove dead entities from the main vec (hint: check your functions)
  // remove dead entities from the map (hint the map stores vectors)
}
std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
  // TODO: create an entity and store in the buffer vector toAdd
  // make sure to create a entity pointer (vector stores pointers)
  // initialize entity with an id number  and tag
  // hint id number should be associated to when the entity is created
  // hint: use total entities
}
const EntityVec &EntityManager::getEntities() const {
  // TODO: should return the entity vector
}

const EntityVec &EntityManager::getEntities(const std::string &tag) {
  // TODO: should return a vector of a given tag
  // hint: use map
}
const EntityMap &EntityManager::getEntityMap() {
  // TODO: return entity map
}
const size_t &EntityManager::getTotalEntities() {
  // TODO: should return total entities
}
