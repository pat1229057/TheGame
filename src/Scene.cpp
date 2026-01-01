#include "Scene.h"

void Scene::setPaused(bool paused) {}

Scene::Scene(GameEngine &game) : m_game(&game) {}
void Scene::simulate(int something) {}
void Scene::registerAction(int inputkey, const std::string &aName) {}
ActionMap &Scene::getActionMap() { return m_actionMap; }
