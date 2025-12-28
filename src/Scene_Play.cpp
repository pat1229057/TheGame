#include "Scene_Play.h"
#include "Action.hpp"
#include "Assets.h"
#include "Components.hpp"
#include "GameEngine.h"
#include "Physics.hpp"
#include <memory>

Scene_Play::Scene_Play(GameEngine &gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath),
      m_gridText(Assets::Instance().getFont("font/path"), "string", 23) {
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {

  // TODO: Register Actions Toggle , quit, and pause

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
}

void Scene_Play::loadLevel(const std::string &filename) {

  // reset the entity manager every time we load a level
  m_entityManager = EntityManager();

  // TODO: read in the level file and add the appropriate entities
  // use the playerConfig struct m_playerConfig to store player properties
  // this stuct is defined at the top of Scene_Play.h

  // NOTE: all of the code below is sample code which shows you how to
  // set up and use entities it should be removed

  spawnPlayer();

  // some sample entities
  auto brick = m_entityManager.addEntity("tile");
  // IMPORTANT: always add the CAnimation component first so that the
  // gridToMidPixel can compile
  // brick->add<CAnimation>(Assets::Instance().getAnimation("Brick"), true);
  // brick->add<CTransform>(Vec2f(96, 480));
  // NOTE: Your final code should position the entity with the grid x,y position
  // read from
  // brick->add<CTransform>(gridToMidPixel(gridX, gridY, brick));
  if (brick->get<CAnimation>().animation.getName() == "Brick") {
    // This could be a good way of identifying if a tile is a brick!
  }
  // component return references use auto&
}

void Scene_Play::spawnPlayer() {

  // check to see if a player already exists before adding a new one
  // if it already exists, just overwrite the values of the existing one

  // add components to player entity

  // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {

  // TODO: this should spawn a bullet at the given entity,
  //  going in the direction the entity is facing
}

void Scene_Play::update() {
  // update entity manager

  // TODO: implement pause functionality

  // run all systems
}

void Scene_Play::sMovement() {
  // TODO: Implement player movement / jumping based on its CInput component
  // TODO: Implement gravity's effect on the player
  // TODO: Implement the maximum player speed in both X and Y directions
  // NOTE: Setting an entity's scale.x to -1/1 will make it face to the
  // left/right
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
  // TODO: Check to see if the player has fallen down a hole (y > height())
  // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action &action) {

  // implement doActions for START and ELSE actions
}

void Scene_Play::sAnimation() {
  // TODO: Complete the Animation class code first

  // TODO: for each entity with an animation, call
  // entity->get<CAnimation>().animation.update()
  //  if the animation is not repeated, and it has ended, destroy the entity

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

  // set the viewpoint of the window to be centered on the player

  // draw all Entity textures / animations

  // draw all Entity collision bounding boxes with a rectangle

  // draw the grid
}

void Scene_Play::drawLine(const Vec2f &p1, const Vec2f &p2) {
  // draw lines loop up sf::Vertex and sf::Lines
}

std::shared_ptr<Entity> Scene_Play::player() {
  // return player entity
}
