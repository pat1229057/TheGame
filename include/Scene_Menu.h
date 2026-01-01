#pragma once

#include "Scene.h"
#include <vector>

class Scene_Menu : public Scene {

protected:
  std::vector<std::string> m_menuStrings;
  sf::Text m_menuText;
  std::vector<std::string> m_levelPaths;
  int m_menuIndex;

public:
  Scene_Menu(GameEngine &gameEngine);

  void init();
  void update() override;

  void sDoAction(const Action &action) override;

  void sRender() override;
};
