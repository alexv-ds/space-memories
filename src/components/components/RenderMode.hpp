#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/BlendMode.hpp>


namespace component {

struct DefaultRenderMode {
  sf::Color color = sf::Color::White; //в json как массив [r,g,b] либо [r,g,b,a]
  sf::BlendMode blend_mode; //в json текст, только ADD, MULTIPLY, ALPHA
};


}
