#pragma once
#include <string_view>
#include <utility>
#include <SFML/Graphics/Texture.hpp>
#include <entt/entity/registry.hpp>
#include <core/Service.hpp>
#include <components/Sprite.hpp>

struct State {
  std::string name;
  std::vector<float> delays;
  std::vector<std::vector<sf::IntRect>> dirs;
  float full_animation_time = 0;
  int frame = 0; //текущий фрейм, если спрайт имеет анимацию
};

namespace service {

class SpriteManager : public core::Service {
public:
  virtual component::SpriteOld load_sprite(std::string_view name, std::string_view state = "") = 0;
  virtual std::pair<sf::Texture*, sf::IntRect> get_texture(const entt::registry&, entt::entity) = 0;
  virtual const State* get_sprite_data(int id, int state) = 0;
  
  virtual void update() noexcept = 0;
  virtual ~SpriteManager() = default;
};


}
