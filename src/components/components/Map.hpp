#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

namespace component {

//C++ only
struct LoadMap {
  std::string file;
  sf::Vector2f offset;
};


}
