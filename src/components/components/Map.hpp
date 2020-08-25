#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

namespace component {

struct LoadMap {
  std::string file;
  sf::Vector2f offset;//в json аrray [x,y]
};


}
