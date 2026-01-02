#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <type_traits>

template <typename T>
concept ArithmeticType = std::is_arithmetic_v<T>;

template <ArithmeticType T> class Vec2 {

public:
  T x{0};
  T y{0};

  constexpr Vec2() = default;
  constexpr Vec2(T xtemp, T ytemp) : x(xtemp), y(ytemp) {}

  explicit Vec2(const sf::Vector2<T> &vec) : x(vec.x), y(vec.y) {}
  explicit Vec2(const sf::Vector2i &vec) : x(T(vec.x)), y(T(vec.y)) {}

  // allows automatic conversion to sf::Vector
  //  this allows you to pass vec2 into functions
  operator sf::Vector2<T>() const { return sf::Vector2<T>(x, y); }

  constexpr Vec2 operator+(const Vec2 &rhs) const noexcept {
    Vec2 result{};
    result.x = this->x + rhs.x;
    result.y = this->y + rhs.y;
    return result;
  }
  constexpr Vec2 operator*(const Vec2 &rhs) const noexcept {
    Vec2 result{};
    result.x = this->x * rhs.x;
    result.y = this->y * rhs.y;
    return result;
  }
  template <ArithmeticType U>
  constexpr Vec2 operator*(const U &rhs) const noexcept {
    Vec2 result{};
    result.x = this->x * rhs;
    result.y = this->y * rhs;
    return result;
  }

  constexpr Vec2 operator-(const Vec2 &rhs) const noexcept {
    Vec2 result{};
    result.x = this->x - rhs.x;
    result.y = this->y - rhs.y;
    return result;
  }
  constexpr Vec2 operator/(const Vec2 &rhs) const noexcept {
    Vec2 result{};
    result.x = this->x / rhs.x;
    result.y = this->y / rhs.y;
    return result;
  }

  constexpr Vec2 &operator+=(const Vec2 &rhs) noexcept {
    this->x = this->x + rhs.x;
    this->y = this->y + rhs.y;
    return *this;
  }
  constexpr Vec2 &operator-=(const Vec2 &rhs) noexcept {
    this->x = this->x - rhs.x;
    this->y = this->y - rhs.y;
    return *this;
  }
  constexpr Vec2 &operator*=(const T val) noexcept {
    this->x = this->x * val;
    this->y = this->y * val;
    return *this;
  }
  constexpr Vec2 &operator/=(const T val) noexcept {
    this->x = this->x / val;
    this->y = this->y / val;
    return *this;
  }

  constexpr bool operator==(const Vec2 &rhs) const noexcept {
    if (this->x == rhs.x && this->y == rhs.y)
      return true;
    else
      return false;
  }
  constexpr bool operator!=(const Vec2 &rhs) const noexcept {
    if (this->x != rhs.x || this->y != rhs.y)
      return true;
    else
      return false;
  }

  T dist(const Vec2 &rhs) const noexcept {
    Vec2 D(rhs.x - this->x, rhs.y - this->y);

    return std::sqrt(D.x * D.x + D.y * D.y);
  }

  T length() const noexcept { return std::sqrt(x * x + y * y); }

  void normalize() noexcept {
    float length = this->length();
    if (length == 0)
      return;
    *this = Vec2(this->x / length, this->y / length);
  }
};

using Vec2f = Vec2<float>;
