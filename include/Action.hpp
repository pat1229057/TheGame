#pragma once

#include "Entity.hpp"
#include <sstream>
#include <string>

class Action {

private:
  std::string m_name{"NONE"};
  std::string m_type = "NONE";

public:
  Action() = default;

  // setup a constructor to initialize variables in class
  Action(const std::string &name, const std::string &type)
      : m_name(name), m_type(type) {}

  // getter function
  const std::string &name() const { return m_name; }

  // getter function
  const std::string &type() const { return m_type; }
};
