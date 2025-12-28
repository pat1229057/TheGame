#include "Scene_Menu.h"
#include "Action.hpp"
#include "Assets.h"
#include "Components.hpp"
#include "GameEngine.h"
#include "Scene_Play.h"

Scene_Menu::Scene_Menu(GameEngine &gameEngine)
    : Scene(gameEngine),
      m_menuText(Assets::Instance().getFont("font/path"), "string", 23) {
  init();
}

void Scene_Menu::init() {

  // TODO: register actions for menu

  // TODO: Initialize menustring vector and set font and character size (64)
}

void Scene_Menu::update() {
  // TODO: update entities
}

void Scene_Menu::sDoAction(const Action &action) {

  // TODO: define logic for actions
}

void Scene_Menu::sRender() {

  // Set window view
  // clear window to blue (100,100,255)

  // render title
  // draw controls
  // draw all menu options
}
