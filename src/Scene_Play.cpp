#include "Scene_Play.h"
#include "Action.hpp"
#include "Animation.hpp"
#include "Assets.h"
#include "Components.hpp"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Physics.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Vec2.hpp"
#include <algorithm>
#include <cstdlib>
#include <istream>
#include <memory>
#include <sstream>
#include <string>

Scene_Play::Scene_Play(GameEngine &gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath),
      m_gridText(Assets::Instance().getFont("Tech"), "string", 23) {
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {

  // TODO: Register Actions Toggle , quit, and pause
  registerAction(sf::Keyboard::Scancode::P, "PAUSE");
  registerAction(sf::Keyboard::Scancode::Escape, "QUIT");
  registerAction(sf::Keyboard::Scancode::T, "TOGGLE_TEXTURE");
  registerAction(sf::Keyboard::Scancode::C, "TOGGLE_COLLISION");
  registerAction(sf::Keyboard::Scancode::G, "TOGGLE_GRID");

  // TODO: Register all other gameplay actions

  loadLevel(levelPath);
}

Vec2f Scene_Play::GridToMidPixel(float gridX, float gridY,
                                 std::shared_ptr<Entity> entity) {

  // TODO: This function takes in a grid (x,y) position and an Entity
  //  return Vec2 indicating where the center position of the entity should be
  //  You must use the entity's animation size to position it correctly
  //                   entity->get<CAnimation>().getSize()
  //  The Size of the grid width and height is stored in the m_gridsize.x and
  //  m_gridsize.y
  //
  // The bottom-left corner of the animation should align with the
  //  bottom left of the grid cell
  //
  //  Remember that SFML has (0,0) in the top left, while grid coordinates are
  //  specified from bottom
  // You can get the size of the SFML window via
  //  m_game.window().getSize();
  Vec2f position{0, 0};
  position +=
      Vec2f(gridX * m_gridSize.x,
            (m_game->window().getView().getSize().y) - gridY * m_gridSize.y);

  position.x += entity->get<CAnimation>().animation.getSize().x / 2.f;
  position.y -= entity->get<CAnimation>().animation.getSize().y / 2.f;
  return position;
}
void Scene_Play::loadLevel(const std::string &filename) {

  // reset the entity manager every time we load a level
  m_entityManager = EntityManager();

  // TODO: read in the level file and add the appropriate entities
  // use the playerConfig struct m_playerConfig to store player properties
  // this stuct is defined at the top of Scene_Play.h

  // NOTE: all of the code below is sample code which shows you how to
  // set up and use entities it should be removed
  std::ifstream file(filename);

  if (!file) {
    std::cerr << "LOAD LEVEL FILE NOT FOUND\n";
    return;
  }
  std::string buffer;
  while (std::getline(file, buffer)) {
    std::stringstream line(buffer);
    std::string type;
    line >> type;
    if (type == "Tile") {
      auto tile = m_entityManager.addEntity(type);

      std::string animationName;
      line >> animationName;
      tile->add<CAnimation>(Assets::Instance().getAnimation(animationName),
                            true);
      Vec2f size(
          static_cast<float>(tile->get<CAnimation>().animation.getSize().x),
          static_cast<float>(tile->get<CAnimation>().animation.getSize().y));
      tile->add<CBoundingBox>(size);
      float gridX, gridY;
      line >> gridX >> gridY;
      Vec2f position = GridToMidPixel(gridX, gridY, tile);
      tile->add<CTransform>(position);
    }
    if (type == "Dec") {

      auto dec = m_entityManager.addEntity(type);

      std::string animationName;
      line >> animationName;
      dec->add<CAnimation>(Assets::Instance().getAnimation(animationName),
                           true);
      Vec2f size(
          static_cast<float>(dec->get<CAnimation>().animation.getSize().x),
          static_cast<float>(dec->get<CAnimation>().animation.getSize().y));
      float gridX, gridY;
      line >> gridX >> gridY;
      Vec2f position = GridToMidPixel(gridX, gridY, dec);
      dec->add<CTransform>(position);
    }
    if (type == "Player") {
      line >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >>
          m_playerConfig.CY >> m_playerConfig.SPEED >> m_playerConfig.JUMP >>
          m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY >>
          m_playerConfig.WEAPON;
      spawnPlayer();
    }
    std::cout << "Loaded " << m_entityManager.getEntities().size()
              << " entities\n";
  }
  file.close();
  std::cout << "Loaded " << m_entityManager.getEntities().size()
            << " entities\n";

  // some sample entities
  // IMPORTANT: always add the CAnimation component first so that the
  // gridToMidPixel can compile
  // brick->add<CAnimation>(Assets::Instance().getAnimation("Brick"), true);
  // brick->add<CTransform>(Vec2f(96, 480));
  // NOTE: Your final code should position the entity with the grid x,y position
  // read from
  // brick->add<CTransform>(gridToMidPixel(gridX, gridY, brick));
  // if (brick->get<CAnimation>().animation.getName() == "Brick") {
  // This could be a good way of identifying if a tile is a brick!
  //}
  // component return references use auto&
}

void Scene_Play::spawnPlayer() {

  // check to see if a player already exists before adding a new one
  // if it already exists, just overwrite the values of the existing one
  if (m_entityManager.getEntities("Player").size() > 0) {
    return;
  }
  // add components to player entity

  // TODO: be sure to add the remaining components to the player

  m_player = m_entityManager.addEntity("Player");
  m_player->add<CAnimation>(Assets::Instance().getAnimation("Stand"), true);
  m_player->add<CBoundingBox>(Vec2f(m_playerConfig.CX, m_playerConfig.CY));
  m_player->add<CTransform>(
      GridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
  m_player->add<CState>();
  m_player->add<CInput>();
  m_player->add<CGravity>(m_playerConfig.GRAVITY);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {

  // TODO: this should spawn a bullet at the given entity,
  //  going in the direction the entity is facing
}

void Scene_Play::update() {
  // update entity manager
  m_entityManager.update();
  // TODO: implement pause functionality
  sRender();
  sAnimation();
  sMovement();
  sCollision();

  // run all systems
}

void Scene_Play::sMovement() {
  // TODO: Implement player movement / jumping based on its CInput component
  // TODO: Implement gravity's effect on the player

  auto &transformComponent = player()->get<CTransform>();
  transformComponent.prevPos = transformComponent.pos;
  float deltaTime = m_game->getDeltaTime().asSeconds();
  transformComponent.velocity.y += m_playerConfig.GRAVITY * deltaTime;
  transformComponent.velocity.y =
      std::min(transformComponent.velocity.y, m_playerConfig.MAXSPEED);

  transformComponent.pos.y += transformComponent.velocity.y * deltaTime;

  // std::cout << player()->get<CTransform>().pos.y << '\n';
  //  TODO: Implement the maximum player speed in both X and Y directions
  //  NOTE: Setting an entity's scale.x to -1/1 will make it face to the
  //  left/right
}

void Scene_Play::sLifespan() {
  // TODO: Check lifespan of entities that have them, and destroy them if they
  // go over
}

void Scene_Play::sCollision() {

  // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
  // this means jumping will have a negative y component
  // and gravity will have a position y-component
  // Also, something BELOW something else will have a y value GREATER than it
  // Also, something ABOVE something else will have a y value LESS than it

  // TODO: Implement Physics::GetOverlap() function, use it inside this function

  // TODO: Implement bullet / tile collisions
  //  Destroy the tile if it has a brick animation
  // TODO: Implement player / tile collisions and resolutions
  //  update the CState component of the player to store whether
  //  it is currently on the gorund or in the air this will be sued by the
  //  animation system
  auto &pos = m_player->get<CTransform>().pos;

  for (auto entity : m_entityManager.getEntities()) {

    if (entity->tag() == "Player" || entity->tag() == "Dec") {
      continue;
    }

    if (entity->tag() == "Tile") {

      Vec2f overlap = Physics::getOverlap(m_player, entity);

      if (overlap.x > 0 && overlap.y > 0) {
        Vec2f previousOverlap = Physics::getPreviousOverlap(m_player, entity);
        std::cout << "prevX: " << previousOverlap.x
                  << " prevY: " << previousOverlap.y << std::endl;

        std::cout << "collision\n";
        if (previousOverlap.y > 0) {

        } else if (previousOverlap.x > 0) {
          std::cout << "hello\n";
          if (auto entityPos = entity->get<CTransform>().pos;
              pos.y < entityPos.y) {
            pos.y -= overlap.y;
            std::cout << "resolution\n";
          } else if (pos.y > entityPos.y) {
          }
        } else if (previousOverlap.x < 0 && previousOverlap.y < 0) {
          pos.y -= overlap.y;
        }
      }
    }
  }
  // TODO: Check to see if the player has fallen down a hole (y > height())
  // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action &action) {

  // implement doActions for START and ELSE actions
  if (action.type() == "PRESS") {

    if (action.name() == "QUIT") {
      m_game->quit();
    }
  }
}

void Scene_Play::sAnimation() {
  // TODO: Complete the Animation class code first

  // TODO: for each entity with an animation, call
  // entity->get<CAnimation>().animation.update()
  //  if the animation is not repeated, and it has ended, destroy the entity
  for (auto entity : m_entityManager.getEntities()) {
    if (entity->has<CAnimation>()) {
      auto &animation = entity->get<CAnimation>();
      animation.animation.update();
    }
  }

  // TODO: set the animation of the player based on its CState component
  // if Cstate is running
  // change animation to repeating
}

void Scene_Play::onEnd() {
  // TODO: When the scene ends, change back to the MENU scene
  // use m_game.changeScene(correct params);
}

void Scene_Play::sGUI() {}

void Scene_Play::sRender() {
  // color the background darker so you know tha tthe game is paused (100, 100,
  // 250) clear to blue by default ( 50, 50, 150)
  if (!m_paused) {
    m_game->window().clear(sf::Color(100, 100, 250));
  } else {
    m_game->window().clear(sf::Color(50, 50, 150));
  }

  // set the viewpoint of the window to be centered on the player
  auto &pos = player()->get<CTransform>().pos;
  float windowCenterX =
      std::max(m_game->window().getView().getSize().x / 2.0f, pos.x);
  sf::View view = m_game->window().getView();
  view.setCenter({windowCenterX, m_game->window().getView().getSize().y / 2.f});
  m_game->window().setView(view);

  // draw all Entity textures / animations
  if (m_drawTextures) {
    for (auto entity : m_entityManager.getEntities()) {
      if (entity->has<CAnimation>()) {
        auto &AnimationComponent = entity->get<CAnimation>();
        sf::Sprite sprite(AnimationComponent.animation.getTexture());
        sprite.setTextureRect(AnimationComponent.animation.getRect());
        sprite.setOrigin({AnimationComponent.animation.getRect().size.x / 2.f,
                          AnimationComponent.animation.getRect().size.y / 2.f});

        sprite.setPosition(entity->get<CTransform>().pos);
        m_game->window().draw(sprite);
      }
    }
  }

  // !most confusing code for em
  if (!m_drawGrid) {
    auto [width, height] = m_game->window().getView().getSize();

    float leftX = m_game->window().getView().getCenter().x - width / 2;
    float rightX = leftX + width + m_gridSize.x;
    float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

    for (float x = nextGridX; x < rightX; x += m_gridSize.y) {
      drawLine(Vec2f(x, 0), Vec2f(x, height));
    }
    for (float y = 0; y < height; y += m_gridSize.y) {
      drawLine(Vec2f(leftX, height - y), Vec2f(rightX, height - y));

      for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
        std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
        std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
        m_gridText.setString("(" + xCell + "," + yCell + ")");
        m_gridText.setPosition({x + 3, height - y - m_gridSize.y + 2});
        m_game->window().draw(m_gridText);
      }
    }
  }
  // draw all Entity collision bounding boxes with a rectangle

  // draw the grid
  m_game->window().display();
}

void Scene_Play::drawLine(const Vec2f &p1, const Vec2f &p2,
                          const sf::Color &color) {
  // draw lines loop up sf::Vertex and sf::Lines
  sf::Vertex line[]{{p1, color, sf::Vector2f()}, {p2, color, {0, 0}}};
  m_game->window().draw(line, 2, sf::PrimitiveType::Lines);
};

std::shared_ptr<Entity> Scene_Play::player() {
  // return player entity
  if (m_entityManager.getEntityMap().find("Player") ==
      m_entityManager.getEntityMap().end()) {
    return nullptr;
  }
  auto player = m_entityManager.getEntities("Player").front();
  return player;
}
