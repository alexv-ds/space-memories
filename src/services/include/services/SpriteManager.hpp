#pragma once
#include <string_view>
#include <SFML/Graphics/Texture.hpp>
#include <core/Service.hpp>
#include <components/Sprite.hpp>
#include <utility>

namespace service {

class SpriteManager : public core::Service {
public:
  virtual int load_sprite(std::string_view name, std::string_view state = "") = 0;
  virtual std::pair<sf::Texture*, sf::IntRect> get_texture(const component::Sprite& sprite) = 0;
};


}
