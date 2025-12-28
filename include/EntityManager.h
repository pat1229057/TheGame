#pragma once
#include "Entity.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <vector>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
  EntityVec m_entities;
  EntityVec m_toAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities = 0;

  void removeDeadEntities(EntityVec &vec);

public:
  void init();
  EntityManager() = default;
  void update();
  std::shared_ptr<Entity> addEntity(const std::string &tag);
  const EntityVec &getEntities() const;
  const EntityVec &getEntities(const std::string &tag);
  const EntityMap &getEntityMap();
  const size_t &getTotalEntities();
};
