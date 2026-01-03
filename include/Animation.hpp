#pragma once
#include "Assets.h"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Vec2.hpp"
#include <cstddef>
#include <string>

class Animation {
private:
  std::string m_name = "none"; // name of the animation
  std::string m_textureName =
      "none"; // name of the texture to get the frames fromsize_t m_frameCount =
              // 1;     // total number of frames of animation
  size_t m_frameCount{1};
  size_t m_currentFrame = 0; // the current frame of animation being played
  size_t m_speed = 0;        // speed to play this animation

  sf::IntRect m_textureRect; // sub rectangle to draw
  Vec2<size_t> m_size;

public:
  Animation() = default;

  // create animation constructor that initialize name and texturename (1,0)?
  Animation(const std::string &name, const std::string &textureName)
      : Animation(name, textureName, 1, 0) {}

  // make a parameter constructor that initialize object (name, framecount,
  // current frame, speed, textureName)
  Animation(const std::string &name, const std::string &textureName,
            size_t frameCount, size_t speed)
      : m_name(name), m_textureName(textureName), m_frameCount(frameCount),
        m_currentFrame(0), m_speed(speed) {

    // TODO: get texture from singleton and set texture rect (sprite) size to
    // the first frame count
    sf::Texture texture{Assets::Instance().getTexture(textureName)};
    m_textureRect.size = {static_cast<int>(texture.getSize().x / frameCount),
                          static_cast<int>(texture.getSize().y / frameCount)};
    m_size.x = m_textureRect.size.x;
    m_size.y = m_textureRect.size.y;
  }

  // update the animation to show the next frame, depending on it
  //  animation loops when it reaches the end
  void update() {

    // TODO: 1) calculate the correct frame of animation to play based on
    // currentFrame
    //      2) set the texture rectangle properly (see constructor for sample)
  }
  bool hasEnded() const {

    // TODO: detect when animation has ended (last frame was played) and return
    // true
  }

  const std::string &getName() const { return m_name; }

  const sf::IntRect &getRect() const { return m_textureRect; }

  template <typename T> const Vec2<T> &getSize() const { return m_size; }

  const sf::Texture &getTexture() {
    return Assets::Instance().getTexture(m_textureName);
  }
};
