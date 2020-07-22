#pragma once

#include <entt/entity/entity.hpp>

namespace component {

struct Position {
  int x = 0;
  int y = 0;
};

struct ScreenPosition {
  entt::entity camera = entt::null;
  float x = 0;
  float y = 0;
};


}