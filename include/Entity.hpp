#pragma once

#include "Components.hpp"
#include <string>
#include <tuple>

using ComponentTuple = std::tuple<CTransform, CBoundingBox, CAnimation,
                                  CGravity, CInput, CLifeSpan, CState>;

class Entity {

  friend class EntityManager;

  ComponentTuple m_components;
  bool m_alive = true;
  std::string m_tag = "default";
  size_t m_id = 0;
  Entity() = default;
  Entity(const size_t &id, const std::string &tag) : m_tag(tag), m_id(id) {}

public:
  template <typename T, typename... TArgs> T &add(TArgs &&...args) noexcept {
    auto &component = get<T>();
    component = T(std::forward<TArgs>(args)...);
    component.exist = true;
    return component;
  }
  template <typename T> const T &get() const noexcept {
    return std::get<T>(m_components);
  }

  template <typename T> T &get() noexcept {
    return std::get<T>(m_components);
  }

  template <typename T> bool has() const noexcept {
    return get<T>().exist;
  }

  size_t id() const noexcept {
    return m_id;
  }

  bool isAlive() const noexcept {
    return m_alive;
  }

  void destroy() noexcept {
    m_alive = false;
  }

  const std::string &tag() const noexcept {
    return m_tag;
  }

  template <typename T> void remove() noexcept {
      auto& C = get<T>();
      C = T();
      get<T>().exist = false;
  }
};
