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
#include <sstream>
#include <string>

Assets &Assets::Instance() {
  static Assets assets;
  return assets;
}

void Assets::addTexture(const std::string &textureName, const std::string &path,
                        bool smooth) {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    std::cout << "ERROR: " << textureName << " " << "Not loaded, Path: " << path
              << " NOT FOUND\n";
    return;
  }
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

void Assets::loadFromFile(const std::string &path) {
  std::ifstream file(path);
  if (!file) {
    std::cerr << "Error Assets.txt not found\n";
  }
  std::string buffer;
  while (std::getline(file, buffer)) {
    std::stringstream line = static_cast<std::stringstream>(buffer);
    std::string type;
    line >> type;
    if (type == "Texture") {
      std::string name, path;
      line >> name >> path;

      addTexture(name, path);

    } else if (type == "Animation") {
      std::string animationName, textureName;
      size_t framecount, speed;
      line >> animationName >> textureName >> framecount >> speed;
      addAnimation(animationName, textureName, framecount, speed);
    } else if (type == "Font") {
      std::string name, path;
      line >> name >> path;
      addFont(name, path);
    }
  }
}

const sf::Texture &Assets::getTexture(const std::string &textureName) const {
  if (m_textureMap.find(textureName) == m_textureMap.end()) {
    std::cout << textureName << " NOT FOUND\n";
  }
  return m_textureMap.at(textureName);
}
const Animation &Assets::getAnimation(const std::string &animationName) const {
  if (m_animationMap.find(animationName) == m_animationMap.end()) {
    std::cout << animationName << " NOT FOUND\n";
  }
  return m_animationMap.at(animationName);
}
const sf::Font &Assets::getFont(const std::string &fontName) const {
  if (m_fontMap.find(fontName) == m_fontMap.end()) {
    std::cout << fontName << " NOT FOUND\n";
  }
  return m_fontMap.at(fontName);
}
const std::map<std::string, sf::Texture> &Assets::getTextures() const {
  return m_textureMap;
}
const std::map<std::string, Animation> &Assets::getAnimations() const {
  return m_animationMap;
}
