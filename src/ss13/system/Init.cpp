#include "Init.hpp"

#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include <core/component/Texture.hpp>
#include <core/component/Position.hpp>
#include <core/component/Renderable.hpp>

namespace ss13::system {

const std::string_view Init::name() const noexcept {
  return "ss13::system::Init";
}

void Init::init() {
  this->texture_loader = core::service::get_service<core::service::TextureLoader>();

  const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0,2);

  //
  for (int x = 0; x < 14; ++x) {
    for (int y = 0; y < 14; ++y) {
      core::TextureID tex_id;
      switch (distribution(generator)) {
        case 0: {
          tex_id = this->texture_loader->load("resources/floor_dark.png");
          break;
        }
        case 1: {
          tex_id = this->texture_loader->load("resources/floor_steel.png");
          break;
        }
        case 2: {
          tex_id = this->texture_loader->load("resources/floor_white.png");
          break;
        }
      }
      entt::entity entity = this->registry->create();

      this->registry->assign<core::component::Position>(entity, x, y);
      this->registry->assign<core::component::Renderable>(entity);
      this->registry->assign<core::component::Texture>(entity, tex_id);
    }
  }
  //end of loop
}

void Init::update() {


}


} /* ss13::system */
