#pragma once

#include <SFML/Graphics/Color.hpp>
#include <entt/entity/entity.hpp>
#include "Position.hpp"

namespace component {

struct Camera {
  float size_x = 1.0;
  float size_y = 1.0;
};

struct BindCameraToRenderWindow {
  entt::entity window = entt::null;
};

struct RenderableQuad {
  sf::Color color = sf::Color::White;
};

}