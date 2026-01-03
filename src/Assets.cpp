#include "Assets.h"
#include "Animation.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>

Assets &Assets::Instance() {
  static Assets assets;
  return assets;
}

void Assets::addTexture(const std::string &textureName, const std::string &path,
                        bool smooth) {
  m_textureMap[textureName] = sf::Texture(path, smooth);
}

// random size_t
void Assets::addAnimation(const std::string &animationName,
                          const std::string &textureName, size_t frameCount,
                          size_t speed) {
  m_animationMap[animationName] =
      Animation(animationName, textureName, frameCount, speed);
}

void Assets::addFont(const std::string &fontName, const std::string &path) {
  m_fontMap[fontName] = sf::Font(path);
}

void Assets::loadFromFile(const std::string &path) {}

const sf::Texture &Assets::getTexture(const std::string &textureName) const {
  return m_textureMap.at(textureName);
}
const Animation &Assets::getAnimation(const std::string &animationName) const {
  return m_animationMap.at(animationName);
}
const sf::Font &Assets::getFont(const std::string &fontName) const {
  return m_fontMap.at(fontName);
}
const std::map<std::string, sf::Texture> &Assets::getTextures() const {
  return m_textureMap;
}
const std::map<std::string, Animation> &Assets::getAnimations() const {
  return m_animationMap;
}
