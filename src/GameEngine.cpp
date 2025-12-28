#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"

#include <iostream>
#include <memory>

GameEngine::GameEngine(const std::string &path) { init(path); }

void GameEngine::init(const std::string &path) {
  // load assets
  // create and initialize window
  // initialize GUI
  // scale imgui
  // set default scene to menu
}

std::shared_ptr<Scene> GameEngine::currentScene() {
  // TODO: return current scene
}

bool GameEngine::isRunning() {
  // TODO: if window is running
}

sf::RenderWindow &GameEngine::window() {
  // return window
}

void GameEngine::run() {

  // TODO: setup game loop
}

void GameEngine::sUserInput() {
  // TODO write Event loop

  // do quit
  // do screenshot x

  // TODO: event pressed or released
  //  error handle if no action is assciated
  //  determine start or end action by wheter it was keypressed or keyreleased
  //  look up the actio and send the action to the scene
}

void GameEngine::changeScene(const std::string &sceneName,
                             std::shared_ptr<Scene> scene,
                             bool endCurrentScene) {}
