#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/BlendMode.hpp>


namespace component {

struct DefaultRenderMode {
  sf::Color color = sf::Color::White;
  sf::BlendMode blend_mode;
};


}
