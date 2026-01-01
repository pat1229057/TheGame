#include "GameEngine.h"
#include "Assets.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "imgui-SFML.h"
#include "imgui.h"

#include <iostream>
#include <memory>
#include <string>

GameEngine::GameEngine(const std::string &path) { init(path); }

void GameEngine::init(const std::string &path) {
  // Might read in config file

  // load assets
  Assets::Instance().loadFromFile(path);

  // create and initialize window
  m_window.create(sf::VideoMode({600, 800}), "TheGame");
  m_window.setFramerateLimit(60);
  // Might change to config variables

  // initialize GUI
  if (!ImGui::SFML::Init(m_window)) {
    std::cerr << "Failed to initialize ImGui\n";
  }
  // scale imgui
  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  // set default scene to menu
  changeScene("MENU", std::make_shared<Scene_Menu>(*this));
}

std::shared_ptr<Scene> GameEngine::currentScene() {
  // TODO: return current scene
  return m_sceneMap.at(m_currentScene);
}

bool GameEngine::isRunning() {
  // TODO: if window is running
  return m_running;
}

sf::RenderWindow &GameEngine::window() {
  // return window
  return m_window;
}

void GameEngine::run() {

  // TODO: setup game loop
  while (isRunning()) {
    update();
  }
}

void GameEngine::sUserInput() {
  // TODO write Event loop
  while (auto event = m_window.pollEvent()) {
    ImGui::SFML::ProcessEvent(m_window, *event);
    // do quit
    if (event->is<sf::Event::Closed>()) {
      quit();
    }
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        quit();
      }
    }

    // do screenshot x
    if (const auto *keypressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
        sf::Texture texture(m_window.getSize());
        texture.update(m_window);
        if (texture.copyToImage().saveToFile("assets/images/screenshot.png")) {
          std::cout << "Screenshot saved\n";
        }
      }
    }

    // TODO: event pressed or released
    if (event->is<sf::Event::KeyPressed>()) {
      auto keyScanncode = event->getIf<sf::Event::KeyPressed>()->scancode;
      if (currentScene()->getActionMap().find(keyScanncode) ==
          currentScene()->getActionMap().end()) {
        continue;
      }
      currentScene()->sDoAction(
          Action(currentScene()->getActionMap().at(keyScanncode), "PRESS"));
    }
    if (event->is<sf::Event::KeyReleased>()) {
      auto keyScanncode = event->getIf<sf::Event::KeyReleased>()->scancode;
      if (currentScene()->getActionMap().find(keyScanncode) ==
          currentScene()->getActionMap().end()) {
        continue;
      }
      currentScene()->sDoAction(
          Action(currentScene()->getActionMap().at(keyScanncode), "RELEASED"));
    }
    if (event->is<sf::Event::MouseButtonPressed>() ||
        event->is<sf::Event::MouseButtonReleased>()) {
      std::string type;
      sf::Mouse::Button scancode;
      if (event->is<sf::Event::MouseButtonReleased>()) {
        type = "RELEASED";
        scancode = event->getIf<sf::Event::MouseButtonReleased>()->button;
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        type = "PRESS";
        scancode = event->getIf<sf::Event::MouseButtonPressed>()->button;
      }

      if (currentScene()->getActionMap().find(scancode) ==
          currentScene()->getActionMap().end()) {
        continue;
      }
      currentScene()->sDoAction(
          Action(currentScene()->getActionMap().at(scancode), "PRESS"));
    }
  }
}

void GameEngine::changeScene(const std::string &sceneName,
                             std::shared_ptr<Scene> scene,
                             bool endCurrentScene) {
  auto tempCurrentScene = m_currentScene;
  if (m_sceneMap.find(sceneName) == m_sceneMap.end()) {
    m_sceneMap.emplace(sceneName, scene);
    m_currentScene = sceneName;
  } else {
    m_currentScene = sceneName;
  }
  if (endCurrentScene) {
    m_sceneMap.erase(tempCurrentScene);
  }
}
void GameEngine::update() {
  sUserInput();
  currentScene()->update();
}

void GameEngine::quit() { m_window.close(); }
