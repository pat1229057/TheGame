#pragma once

#include "Components.hpp"
#include "Entity.hpp"
#include "Vec2.hpp"
#include <cstdlib>

namespace Physics {

Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

  // TODO: return the overlap rectangle size of the bounding boxe of entity a
  // and b
  const auto &aBoundingbox = a->get<CBoundingBox>();
  const auto &bBoundingbox = b->get<CBoundingBox>();

  Vec2f overlap(0, 0);
  overlap.x = aBoundingbox.halfSize.x + bBoundingbox.halfSize.x -
              std::abs(a->get<CTransform>().pos.x - b->get<CTransform>().pos.x);

  overlap.y = aBoundingbox.halfSize.y + bBoundingbox.halfSize.y -
              std::abs(a->get<CTransform>().pos.y - b->get<CTransform>().pos.y);

  return overlap;
}

Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {

  // TODO: return the previous overlap rectangle size of the bounding boxes of
  // entity
  //  previous overlap uses the entity's previous position
  const auto &aBoundingbox = a->get<CBoundingBox>();
  const auto &bBoundingbox = b->get<CBoundingBox>();

  Vec2f overlap(0, 0);
  overlap.x =
      aBoundingbox.halfSize.x + bBoundingbox.halfSize.x -
      std::abs(a->get<CTransform>().prevPos.x - b->get<CTransform>().prevPos.x);

  overlap.y =
      aBoundingbox.halfSize.y + bBoundingbox.halfSize.y -
      std::abs(a->get<CTransform>().prevPos.y - b->get<CTransform>().prevPos.y);

  return overlap;
}

} // namespace Physics
