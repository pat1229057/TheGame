#include "Scene_Play.h"
#include "Action.hpp"
#include "Animation.hpp"
#include "Assets.h"
#include "Components.hpp"
#include "Entity.hpp"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Physics.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Scene_Menu.h"
#include "Vec2.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
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
  registerAction(sf::Keyboard::Scancode::D, "RUNRIGHT");
  registerAction(sf::Keyboard::Scancode::A, "RUNLEFT");
  registerAction(sf::Keyboard::Scancode::Space, "JUMP");
  registerAction(sf::Mouse::Button::Left, "SHOOT");

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
  }
  file.close();

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

void Scene_Play::spawnBullet(const std::shared_ptr<Entity> &entity) {

  // TODO: this should spawn a bullet at the given entity,
  //  going in the direction the entity is facing
  if (entity->get<CState>().state != "JUMP" && entity->get<CInput>().canShoot) {
    auto bullet = m_entityManager.addEntity("Bullet");
    bullet->add<CLifeSpan>(15, m_currentFrame);
    bullet->add<CAnimation>(Assets::Instance().getAnimation("Buster"), true);
    bullet->add<CTransform>(entity->get<CTransform>().pos);
    auto &transform = bullet->get<CTransform>();
    transform.velocity = entity->get<CTransform>().scale * Vec2f(45.0F, 0.0F);
    transform.scale = entity->get<CTransform>().scale * Vec2f(-1.0F, -1.0F);
    auto [x, y] = bullet->get<CAnimation>().animation.getSize();
    bullet->add<CBoundingBox>(Vec2f(x, y));
    entity->get<CInput>().shoot = false;
  }
}

void Scene_Play::update() {
  // update entity manager
  m_entityManager.update();
  // TODO: implement pause functionality
  sGUI();
  sAnimation();
  sMovement();
  sCollision();
  if (m_player->get<CInput>().shoot) {
    spawnBullet(m_player);
  }
  sLifespan();
  sRender();
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

  auto &inputComponent = m_player->get<CInput>();
  if (inputComponent.right) {
    transformComponent.velocity.x =
        std::min(m_playerConfig.SPEED * 1, m_playerConfig.MAXSPEED);
    transformComponent.scale.x = 1;
    m_player->get<CState>().state = "RUN";
  }
  if (inputComponent.left) {
    transformComponent.velocity.x = m_playerConfig.SPEED * -1;
    m_player->get<CState>().state = "RUN";
    transformComponent.scale.x = -1;
  }
  if (inputComponent.canJump && inputComponent.up) {
    transformComponent.velocity.y = m_playerConfig.JUMP * 1;
    transformComponent.velocity.y =
        std::min(transformComponent.velocity.y, m_playerConfig.MAXSPEED);
    m_player->get<CState>().state = "JUMP";
  }
  if (!inputComponent.right && !inputComponent.left && !inputComponent.up) {
    transformComponent.velocity.x = 0;
    m_player->get<CState>().state = "STAND";
  }

  // std::cout << player()->get<CTransform>().pos.y << '\n';
  //  TODO: Implement the maximum player speed in both X and Y directions
  //  NOTE: Setting an entity's scale.x to -1/1 will make it face to the
  //  left/right
  for (const auto &entity : m_entityManager.getEntities()) {
    auto &transformME = entity->get<CTransform>();
    transformME.pos += transformME.velocity * deltaTime;
  }
}

void Scene_Play::sLifespan() {
  // TODO: Check lifespan of entities that have them, and destroy them if they
  // go over
  for (const auto &entity : m_entityManager.getEntities()) {
    if (entity->has<CLifeSpan>()) {
      auto &lifespanComponent = entity->get<CLifeSpan>();
      if (lifespanComponent.lifespan == 0) {
        entity->destroy();
      } else if (lifespanComponent.lifespan > 0) {
        lifespanComponent.lifespan -= 1;
      }
    }
  }
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
  std::shared_ptr<Entity> bulletptr = nullptr;

  for (const auto &entity : m_entityManager.getEntities()) {

    if (entity->tag() == "Player" || entity->tag() == "Dec") {
      continue;
    }

    if (entity->tag() == "Tile") {

      Vec2f overlap = Physics::getOverlap(m_player, entity);
      Vec2f bulletOverlap;
      for (auto &bullet : m_entityManager.getEntities()) {
        if (bullet->tag() == "Bullet") {
          bulletOverlap = Physics::getOverlap(bullet, entity);
          if (bulletOverlap.x > 0 && bulletOverlap.y > 0) {
            bulletptr = bullet;
          }
        }
      }

      if (overlap.x > 0 && overlap.y > 0) {
        Vec2f previousOverlap = Physics::getPreviousOverlap(m_player, entity);

        if (previousOverlap.y > 0) {
          float prevX = m_player->get<CTransform>().prevPos.x;
          pos.x = prevX;

        } else if (previousOverlap.x > 0) {
          if (auto entityPos = entity->get<CTransform>().pos;
              pos.y < entityPos.y) {
            pos.y -= overlap.y;
            m_player->get<CInput>().canJump = true;

          } else if (pos.y > entityPos.y) {
            pos.y += overlap.y;
            // spawn coin

            if (entity->has<CAnimation>()) {
              if (entity->get<CAnimation>().animation.getName() == "Question") {
                auto coin = m_entityManager.addEntity("Coin");
                coin->add<CAnimation>(Assets::Instance().getAnimation("Coin"),
                                      false);

                auto [x, y] = entityPos;
                coin->add<CTransform>(Vec2f(x, y - 64));
                auto &animation = entity->get<CAnimation>();
                animation.animation =
                    Assets::Instance().getAnimation("Question2");
              }
              if (entity->get<CAnimation>().animation.getName() == "Brick") {
                auto explosion = m_entityManager.addEntity("Dec");
                explosion->add<CAnimation>(
                    Assets::Instance().getAnimation("Explosion"), false);
                explosion->add<CTransform>(entity->get<CTransform>().pos);
                entity->destroy();
                m_player->get<CInput>().up = false;
              }
            }
            if (entity->get<CAnimation>().animation.getName() == "Pole" ||
                entity->get<CAnimation>().animation.getName() == "PoleTop") {
              m_player->get<CTransform>().pos =
                  GridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
            }
          }
        } else if (previousOverlap.x < 0 && previousOverlap.y < 0) {
          auto playerTransform = m_player->get<CTransform>();
          auto entityTransform = entity->get<CTransform>();

          if (playerTransform.prevPos.x < entityTransform.pos.x &&
              playerTransform.prevPos.y > entityTransform.pos.y) {
            // bottom left
            pos.x -= overlap.x;
            pos.y += overlap.y;
          } else if (playerTransform.prevPos.x > entityTransform.pos.x &&
                     playerTransform.prevPos.y > entityTransform.pos.y) {
            // bottom right
            pos.x += overlap.x;
            pos.y += overlap.y;
          } else if (playerTransform.prevPos.x < entityTransform.pos.x &&
                     playerTransform.prevPos.y < entityTransform.pos.y) {
            // top left
            pos.x -= overlap.x;
            pos.y -= overlap.y;
          } else if (playerTransform.prevPos.x > entityTransform.pos.x &&
                     playerTransform.prevPos.y < entityTransform.pos.y) {
            // top right
            pos.x += overlap.x;
            pos.y -= overlap.y;
          }
          // pos.y -= overlap.y;
          // m_player->get<CInput>().canJump = true;
          // std::cout << "Can jump:" << m_player->get<CInput>().canJump <<
          // "\n";
        }
      }
      if (bulletOverlap.x > 0 && bulletOverlap.y > 0) {
        if (entity->get<CAnimation>().animation.getName() == "Brick") {
          auto explosion = m_entityManager.addEntity("Dec");
          explosion->add<CAnimation>(
              Assets::Instance().getAnimation("Explosion"), false);
          explosion->add<CTransform>(entity->get<CTransform>().pos);
          entity->destroy();
          bulletptr->destroy();
        } else {
          bulletptr->destroy();
        }
        if (entity->get<CAnimation>().animation.getName() == "Pole" ||
            entity->get<CAnimation>().animation.getName() == "PoleTop") {
          m_player->get<CTransform>().pos =
              GridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
        }
      }
      // ###############################################

      // ###############################
    }
  }
  // TODO: Check to see if the player has fallen down a hole (y > height())
  // TODO: Don't let the player walk off the left side of the map
  if (pos.y > m_game->window().getView().getCenter().y +
                  m_game->window().getView().getSize().y / 2) {
    m_player->get<CTransform>().pos =
        GridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
  }
}

void Scene_Play::sDoAction(const Action &action) {

  // implement doActions for START and ELSE actions
  if (action.type() == "PRESS") {

    if (action.name() == "QUIT") {
      m_game->quit();
    }
    if (action.name() == "RUNRIGHT") {
      m_player->get<CInput>().right = true;
    }
    if (action.name() == "RUNLEFT") {
      m_player->get<CInput>().left = true;
    }
    if (action.name() == "JUMP") {
      m_player->get<CInput>().up = true;
    }
    if (action.name() == "JUMP") {
      m_player->get<CInput>().up = true;
    }
    if (action.name() == "SHOOT") {
      m_player->get<CInput>().shoot = true;
    }
    if (action.name() == "TOGGLE_COLLISION") {
      m_drawCollision = !m_drawCollision;
    }
    if (action.name() == "TOGGLE_TEXTURE") {
      m_drawTextures = !m_drawTextures;
    }
    if (action.name() == "TOGGLE_GRID") {
      m_drawGrid = !m_drawGrid;
    }

  } else if (action.type() == "RELEASED") {
    if (action.name() == "RUNRIGHT") {
      m_player->get<CInput>().right = false;
    }
    if (action.name() == "RUNLEFT") {
      m_player->get<CInput>().left = false;
    }
    if (action.name() == "JUMP") {
      m_player->get<CInput>().up = false;
      m_player->get<CInput>().canJump = false;
    }
    if (action.name() == "SHOOT") {
      m_player->get<CInput>().shoot = false;
    }
  }
}

void Scene_Play::sAnimation() {
  // TODO: Complete the Animation class code first

  // TODO: for each entity with an animation, call
  // entity->get<CAnimation>().animation.update()
  //  if the animation is not repeated, and it has ended, destroy the entity
  if (m_player->get<CState>().state == "RUN") {
    if (!(m_player->get<CAnimation>().animation.getName() == "Run")) {
      m_player->get<CAnimation>().animation =
          Assets::Instance().getAnimation("Run");
      m_player->get<CAnimation>().repeat = true;
    }
  }
  if (m_player->get<CState>().state == "STAND") {
    if (!(m_player->get<CAnimation>().animation.getName() == "Stand")) {
      m_player->get<CAnimation>().animation =
          Assets::Instance().getAnimation("Stand");
      m_player->get<CAnimation>().repeat = false;
    }
  }
  if (m_player->get<CState>().state == "JUMP") {
    if (!(m_player->get<CAnimation>().animation.getName() == "Air")) {
      m_player->get<CAnimation>().animation =
          Assets::Instance().getAnimation("Air");
      m_player->get<CAnimation>().repeat = false;
    }
  }

  for (const auto &entity : m_entityManager.getEntities()) {
    if (entity->has<CAnimation>()) {
      auto &animation = entity->get<CAnimation>();
      animation.animation.update();
      if (!animation.repeat && animation.animation.hasEnded()) {
        entity->destroy();
      }
    }
  }

  // TODO: set the animation of the player based on its CState component
  // if Cstate is running
  // change animation to repeating
}

void Scene_Play::onEnd() {
  // TODO: When the scene ends, change back to the MENU scene
  // use m_game.changeScene(correct params);

  m_game->changeScene("MENU", std::make_shared<Scene_Menu>(*m_game), true);
}

void Scene_Play::sGUI() {
  ImGui::Begin("GUI");

  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("Task", tab_bar_flags)) {

    if (ImGui::BeginTabItem("Actions")) {
      auto &playerInput = m_player->get<CInput>();
      if (ImGui::Button("START##1")) {
        playerInput.left = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##2")) {
        playerInput.left = false;
      }
      ImGui::SameLine();
      ImGui::Text("LEFT");

      if (ImGui::Button("START##3")) {
        playerInput.right = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##4")) {
        playerInput.right = false;
      }
      ImGui::SameLine();
      ImGui::Text("RIGHT");

      if (ImGui::Button("START##5")) {
        playerInput.up = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##6")) {
        playerInput.up = false;
      }
      ImGui::SameLine();
      ImGui::Text("JUMP");

      if (ImGui::Button("START##7")) {
        playerInput.shoot = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##8")) {
        playerInput.shoot = false;
      }
      ImGui::SameLine();
      ImGui::Text("SHOOT");

      if (ImGui::Button("START##9")) {
      }
      ImGui::SameLine();
      if (ImGui::Button("END##10")) {
      }
      ImGui::SameLine();
      ImGui::Text("PAUSE");

      if (ImGui::Button("START##11")) {
        m_drawCollision = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##12")) {
        m_drawCollision = false;
      }
      ImGui::SameLine();
      ImGui::Text("TOGGLE_COLLISION");

      if (ImGui::Button("START##13")) {
        m_drawGrid = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##14")) {
        m_drawGrid = false;
      }
      ImGui::SameLine();
      ImGui::Text("TOGGLE_GRID");

      if (ImGui::Button("START##15")) {
        m_drawTextures = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("END##16")) {
        m_drawTextures = false;
      }
      ImGui::SameLine();
      ImGui::Text("TOGGLE_TEXTURE");

      if (ImGui::Button("START##17")) {
        onEnd();
      }
      ImGui::SameLine();
      if (ImGui::Button("END##18")) {
        onEnd();
      }

      ImGui::SameLine();
      ImGui::Text("QUIT");

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Assets")) {
      if (ImGui::CollapsingHeader("Animations")) {
        ImGui::Indent();
        std::string textureName;
        sf::Texture texture;

        for (const auto &[textureName, texture] :
             Assets::Instance().getTextures()) {

          ImTextureID textureID =
              (ImTextureID)(intptr_t)texture.getNativeHandle();
          ImGui::Image(textureID, ImVec2(64, 64));
        }
        ImGui::Unindent();
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Systems")) {
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("EntityManager")) {
      if (ImGui::CollapsingHeader("Entities by Tag")) {
        ImGui::Indent();
        if (ImGui::CollapsingHeader("bullet")) {
          ImGui::Indent();
          for (const auto &entity : m_entityManager.getEntities("Bullet")) {
            // ImGui::PushID(static_cast<int>(entity->id()));
            // ImGui::PopID();
            if (entity->has<CAnimation>()) {
              ImTextureID textureID =
                  (ImTextureID)(intptr_t)entity->get<CAnimation>()
                      .animation.getTexture()
                      .getNativeHandle();

              ImGui::Image(textureID, ImVec2(32, 32));
            }
            ImGui::SameLine();
            ImGui::Text("%zu", entity->id());
            ImGui::SameLine();
            std::string tag = entity->tag();
            const char *ctag = tag.c_str();
            ImGui::Text(ctag);
            ImGui::SameLine();
            ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
          }

          ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("coin")) {
          ImGui::Indent();
          for (const auto &entity : m_entityManager.getEntities("Coin")) {
            // ImGui::PushID(static_cast<int>(entity->id()));
            // ImGui::PopID();
            if (entity->has<CAnimation>()) {
              ImTextureID textureID =
                  (ImTextureID)(intptr_t)entity->get<CAnimation>()
                      .animation.getTexture()
                      .getNativeHandle();

              ImGui::Image(textureID, ImVec2(32, 32));
            }
            ImGui::SameLine();
            ImGui::Text("%zu", entity->id());
            ImGui::SameLine();
            std::string tag = entity->tag();
            const char *ctag = tag.c_str();
            ImGui::Text(ctag);
            ImGui::SameLine();
            ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
          }
          ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("dec")) {
          ImGui::Indent();
          for (const auto &entity : m_entityManager.getEntities("Dec")) {
            // ImGui::PushID(static_cast<int>(entity->id()));
            // ImGui::PopID();
            if (entity->has<CAnimation>()) {
              ImTextureID textureID =
                  (ImTextureID)(intptr_t)entity->get<CAnimation>()
                      .animation.getTexture()
                      .getNativeHandle();

              ImGui::Image(textureID, ImVec2(32, 32));
            }
            ImGui::SameLine();
            ImGui::Text("%zu", entity->id());
            ImGui::SameLine();
            std::string tag = entity->tag();
            const char *ctag = tag.c_str();
            ImGui::Text(ctag);
            ImGui::SameLine();
            ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
          }
          ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("player")) {
          ImGui::Indent();
          for (const auto &entity : m_entityManager.getEntities("Player")) {
            // ImGui::PushID(static_cast<int>(entity->id()));
            // ImGui::PopID();
            if (entity->has<CAnimation>()) {
              ImTextureID textureID =
                  (ImTextureID)(intptr_t)entity->get<CAnimation>()
                      .animation.getTexture()
                      .getNativeHandle();

              ImGui::Image(textureID, ImVec2(32, 32));
            }
            ImGui::SameLine();
            ImGui::Text("%zu", entity->id());
            ImGui::SameLine();
            std::string tag = entity->tag();
            const char *ctag = tag.c_str();
            ImGui::Text(ctag);
            ImGui::SameLine();
            ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
          }
          ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("tile")) {
          ImGui::Indent();
          for (const auto &entity : m_entityManager.getEntities("Tile")) {
            // ImGui::PushID(static_cast<int>(entity->id()));
            // ImGui::PopID();
            if (entity->has<CAnimation>()) {
              ImTextureID textureID =
                  (ImTextureID)(intptr_t)entity->get<CAnimation>()
                      .animation.getTexture()
                      .getNativeHandle();

              ImGui::Image(textureID, ImVec2(32, 32));
            }
            ImGui::SameLine();
            ImGui::Text("%zu", entity->id());
            ImGui::SameLine();
            std::string tag = entity->tag();
            const char *ctag = tag.c_str();
            ImGui::Text(ctag);
            ImGui::SameLine();
            ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
          }
          ImGui::Unindent();
        }

        ImGui::Unindent();
      }
      if (ImGui::CollapsingHeader("All Entities")) {
        ImGui::Indent();
        for (const auto &entity : m_entityManager.getEntities()) {
          // ImGui::PushID(static_cast<int>(entity->id()));
          // ImGui::PopID();
          if (entity->has<CAnimation>()) {
            ImTextureID textureID =
                (ImTextureID)(intptr_t)entity->get<CAnimation>()
                    .animation.getTexture()
                    .getNativeHandle();

            ImGui::Image(textureID, ImVec2(32, 32));
          }
          ImGui::SameLine();
          ImGui::Text("%zu", entity->id());
          ImGui::SameLine();
          std::string tag = entity->tag();
          const char *ctag = tag.c_str();
          ImGui::Text(ctag);
          ImGui::SameLine();
          ImGui::Text(entity->get<CAnimation>().animation.getName().c_str());
        }
        ImGui::Unindent();
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
}

void Scene_Play::sRender() {
  // color the background darker so you know tha tthe game is paused (100,
  // 100, 250) clear to blue by default ( 50, 50, 150)
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
    for (const auto &entity : m_entityManager.getEntities()) {
      if (entity->has<CAnimation>()) {
        auto &AnimationComponent = entity->get<CAnimation>();
        sf::Sprite sprite(AnimationComponent.animation.getTexture());
        sprite.setTextureRect(AnimationComponent.animation.getRect());
        sprite.setOrigin({AnimationComponent.animation.getRect().size.x / 2.f,
                          AnimationComponent.animation.getRect().size.y / 2.f});

        sprite.setPosition(entity->get<CTransform>().pos);
        if (entity->tag() == "Player") {
          if (entity->get<CInput>().left) {
            sprite.setScale({-1, 1});
          }
        }
        if (entity->tag() == "Bullet") {
          sprite.setScale(entity->get<CTransform>().scale);
        }

        m_game->window().draw(sprite);
      }
    }
  }

  // !most confusing code for em
  if (m_drawGrid) {
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
  if (m_drawCollision) {

    for (const auto &entity : m_entityManager.getEntities()) {
      if (entity->has<CBoundingBox>() && entity->has<CTransform>()) {

        Vec2f pos = entity->get<CTransform>().pos;
        const auto &boundingBox = entity->get<CBoundingBox>();

        Vec2f topleft(pos.x - boundingBox.halfSize.x,
                      pos.y - boundingBox.halfSize.y);
        Vec2f topright(pos.x + boundingBox.halfSize.x,
                       pos.y - boundingBox.halfSize.y);
        Vec2f bottomleft(pos.x - boundingBox.halfSize.x,
                         pos.y + boundingBox.halfSize.y);
        Vec2f bottomright(pos.x + boundingBox.halfSize.x,
                          pos.y + boundingBox.halfSize.y);

        drawLine(topleft, topright);
        drawLine(topright, bottomright);
        drawLine(bottomright, bottomleft);
        drawLine(bottomleft, topleft);
      }
    }
  }

  // draw the grid
  ImGui::SFML::Render(m_game->window());
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
