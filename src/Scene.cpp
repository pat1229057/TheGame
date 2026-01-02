#include "Scene.h"

void Scene::setPaused(bool paused) {}
ActionMap &Scene::getActionMap() { return m_actionMap; }
Scene::Scene(GameEngine &game) : m_game(&game) {}
void Scene::simulate(int something) {}
void Scene::registerAction(int inputkey, const std::string &aName) {}
