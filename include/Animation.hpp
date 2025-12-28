#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Vec2.hpp"
#include <string>

class Animation {
private:
  size_t m_frameCount = 1;     // total number of frames of animation
  size_t m_currentFrame = 0;   // the current frame of animation being played
  size_t m_speed = 0;          // speed to play this animation
  std::string m_name = "none"; // name of the animation
  std::string m_textureName =
      "none";                // name of the texture to get the frames from
  sf::IntRect m_textureRect; // sub rectangle to draw
  Vec2<size_t> m_size;

public:
  Animation() = default;

  // create animation constructor that initialize name and texturename (1,0)?
  Animation(std::string name, std::string textureName)
      : m_name(name), m_textureName(textureName) {}

  // make a parameter constructor that initialize object (name, framecount,
  // current frame, speed, textureName)
  Animation(std::string name, size_t frameCount, size_t currentFrame,
            size_t speed, std::string textureName)
      : m_frameCount(frameCount), m_currentFrame(currentFrame), m_speed(speed),
        m_name(name), m_textureName(textureName) {

    // TODO: get texture from singleton and set texture rect (sprite) size
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

  const std::string &getName() const {}

  const sf::IntRect &getRect() const {}

  template <typename T> const Vec2<T> &getSize() const {}

  sf::Texture &getTexture() {}
};
