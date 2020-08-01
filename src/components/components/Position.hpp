#pragma once

#include <entt/entity/entity.hpp>

namespace component {

struct Position {
  float x = 0.0f;
  float y = 0.0f;
  float layer = 0.0f;
};

struct ScreenPosition {
  entt::entity camera = entt::null;
  float x = 0.0f;
  float y = 0.0f;
};

struct Body {
  float size_x = 1.0f;
  float size_y = 1.0f;
};


}