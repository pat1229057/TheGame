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
  Action(std::string name, std::string type) : m_name(name), m_type(type) {}

  // getter function
  const std::string &name() const {}

  // getter function
  const std::string &type() const {}
};
