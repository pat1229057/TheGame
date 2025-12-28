#pragma once

#include "Action.hpp"
#include "Components.hpp"
#include "EntityManager.h"
#include <memory>

using ActionMap = std::map<int, std::string>;

class GameEngine;

class Scene {

protected:
  GameEngine *m_game = nullptr;
  EntityManager m_entityManager;
  ActionMap m_actionMap;
  bool m_paused = false;
  bool m_hasEnded{false};
  size_t m_currentFrame{0};

  virtual void update() = 0;
  virtual void sDoAction(const Action &action) = 0;
  virtual void sRender() = 0;
  void setPaused(bool paused);

public:
  Scene(GameEngine &game);
  void simulate(int something);
  void registerAction(int inputkey, const std::string &aName);
  virtual ~Scene() = default;
};
