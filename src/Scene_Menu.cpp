#include "Scene_Menu.h"
#include "Action.hpp"
#include "Assets.h"
#include "Components.hpp"
#include "GameEngine.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Scene_Play.h"
#include <memory>

Scene_Menu::Scene_Menu(GameEngine &gameEngine)
    : Scene(gameEngine),
      m_menuText(Assets::Instance().getFont("Tech"), "string", 23) {
  init();
}

void Scene_Menu::init() {

  // TODO: register actions for menu
  registerAction(sf::Keyboard::Scancode::W, "UP");
  registerAction(sf::Keyboard::Scancode::S, "DOWN");
  registerAction(sf::Mouse::Button::Left, "SELECT");
  // TODO: Initialize menustring vector and set font and character size (64)
  m_title = "GFAME";
  m_levelPaths.emplace_back("game_data/level1.txt");
  m_levelPaths.emplace_back("game_data/level2.txt");
  m_levelPaths.emplace_back("game_data/level3.txt");
  m_menuStrings.emplace_back("LEVEL: 1");
  m_menuStrings.emplace_back("LEVEL: 2");
  m_menuStrings.emplace_back("LEVEL: 3");
}

void Scene_Menu::update() {
  // TODO: update entities
  m_entityManager.update();
  sRender();
}

void Scene_Menu::sDoAction(const Action &action) {

  // TODO: define logic for actions
  std::string name = action.name();
  std::string type = action.type();
  static sf::Clock clock;
  if (clock.getElapsedTime().asSeconds() > 0.20f) {
    if (name == "UP") {
      if (m_menuIndex == 0) {
        m_menuIndex = m_menuStrings.size() - 1;
      } else {
        m_menuIndex--;
      }
    }
    if (name == "DOWN") {
      if (m_menuIndex == m_menuStrings.size() - 1) {
        m_menuIndex = 0;
      } else {
        m_menuIndex++;
      }
    }
    if (name == "SELECT") {

      m_game->changeScene("PLAY", std::make_shared<Scene_Play>(
                                      *m_game, m_levelPaths[m_menuIndex]));
    }
    clock.restart();
  }
}

void Scene_Menu::sRender() {

  // Set window view
  m_game->window();
  // clear window to blue (100,100,255)
  m_game->window().clear(sf::Color(100, 100, 255));
  // render title
  m_menuText.setString(m_title);
  m_menuText.setCharacterSize(36);
  m_menuText.setOutlineThickness(4.5f);
  m_menuText.setOrigin({m_menuText.getLocalBounds().size.x / 2,
                        m_menuText.getLocalBounds().size.y / 2});
  m_menuText.setPosition(
      {static_cast<float>(m_game->window().getSize().x) / 2.f,
       static_cast<float>(m_menuText.getCharacterSize())});
  m_menuText.setFillColor(sf::Color(100, 43, 234));
  m_menuText.setOutlineColor(sf::Color::White);
  m_game->window().draw(m_menuText);

  // draw controls
  // draw all menu options
  for (size_t i{0}; i < m_menuStrings.size(); ++i) {
    m_menuText.setOutlineThickness(0.f);
    m_menuText.setString(m_menuStrings[i]);
    m_menuText.setCharacterSize(26);
    m_menuText.setOrigin({m_menuText.getLocalBounds().size.x / 2,
                          m_menuText.getLocalBounds().size.y / 2});
    m_menuText.setPosition(
        {static_cast<float>(m_game->window().getSize().x) / 2.f,
         static_cast<float>(300 + (26 * i))});
    m_menuText.setFillColor(sf::Color::Black);
    if (m_menuIndex == i) {
      m_menuText.setFillColor(sf::Color::Blue);
      m_menuText.setOutlineColor(sf::Color::White);
      m_menuText.setOutlineThickness(4.5f);
    }
    m_game->window().draw(m_menuText);
  }

  m_game->window().display();
}
