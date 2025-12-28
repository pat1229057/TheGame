#include "EntityManager.h"
#include <algorithm>

void EntityManager::removeDeadEntities(EntityVec &vec) {}

// void EntityManager::init() {}
void EntityManager::update() {}
std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {}
const EntityVec &EntityManager::getEntities() const {}

const EntityVec &EntityManager::getEntities(const std::string &tag) {}
const EntityMap &EntityManager::getEntityMap() {}
const size_t &EntityManager::getTotalEntities() {}
