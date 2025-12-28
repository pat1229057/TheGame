#include "Assets.h"
#include "Animation.hpp"
#include "SFML/Graphics.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>

Assets &Assets::Instance() {
  static Assets assets;
  return assets;
}

void Assets::addTexture(const std::string &textureName, const std::string &path,
                        bool smooth) {}

// random size_t
void Assets::addAnimation(const std::string &AnimationName,
                          const std::string &path, size_t) {}

void Assets::addFont(const std::string &fontName, const std::string &path) {}

void Assets::loadFromFile(const std::string &path) {}

const sf::Texture &Assets::getTexture(const std::string &textureName) const {}
const Animation &Assets::getAnimation(const std::string &animationName) const {}
const sf::Font &Assets::getFont(const std::string &fontName) const {}
const std::map<std::string, sf::Texture> &Assets::getTextures() const {}
const std::map<std::string, Animation> &Assets::getAnimations() const {}
