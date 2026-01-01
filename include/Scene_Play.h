#pragma once

#include "EntityManager.h"
#include "Scene.h"
#include <map>
#include <memory>

class Scene_Play : public Scene {

private:
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
    std::string WEAPON;
  };

protected:
  std::shared_ptr<Entity> m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  bool m_drawTextures = true;
  bool m_drawCollision = false;
  bool m_drawGrid = false;
  const Vec2f m_gridSize = {64, 64};
  sf::Text m_gridText;

  void init(const std::string &levelPath);
  Scene_Play(GameEngine &gameEngine, const std::string &levelPath);

  void loadLevel(const std::string &filename);

  void update() override;
  void onEnd();
  void spawnBullet(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> player();
  void spawnPlayer();
  Vec2f GridToMidPixel(float gridX, float gridY,
                       std::shared_ptr<Entity> entity);

  void sCollision();
  void sLifespan();
  void sMovement();
  void sDoAction(const Action &action) override;
  void sAnimation();
  void sGUI();
  void sRender() override;
  void drawLine(const Vec2f &p1, const Vec2f &p2);
};
