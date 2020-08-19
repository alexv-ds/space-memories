#pragma once
#include <string_view>
#include <SFML/Graphics/Texture.hpp>
#include <core/Service.hpp>
#include <components/Sprite.hpp>
#include <utility>

struct State {
  std::string name;
  std::vector<float> delays;
  std::vector<std::vector<sf::IntRect>> dirs;
};

namespace service {

class SpriteManager : public core::Service {
public:
  virtual component::Sprite load_sprite(std::string_view name, std::string_view state = "") = 0;
  virtual std::pair<sf::Texture*, sf::IntRect> get_texture(const component::Sprite& sprite) = 0;
  virtual const State* get_sprite_data(int id, int state) = 0;
  virtual ~SpriteManager() = default;
};


}
