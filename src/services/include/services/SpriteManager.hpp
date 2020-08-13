#pragma once
#include <string_view>
#include <SFML/Graphics/Texture.hpp>
#include <core/Service.hpp>
#include <components/Sprite.hpp>

namespace service {

class SpriteManager : public core::Service {
public:
  virtual int load_sprite(std::string_view name, std::string_view state = "") = 0;
  virtual sf::Texture* get_texture(int id) = 0;
};


}