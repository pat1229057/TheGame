#pragma once

#include "Entity.hpp"

namespace Physics {

Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

  // TODO: return the overlap rectangle size of the bounding boxe of entity a
  // and b
}

Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

  // TODO: return the previous overlap rectangle size of the bounding boxes of
  // entity
  //  previous overlap uses the entity's previous position
}

} // namespace Physics
