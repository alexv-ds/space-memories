#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <components/Sprite.hpp>
#include <services/Time.hpp>
#include <services/SpriteManager.hpp>
#include <iostream>

namespace {

class SpriteFrameAnimation : public core::System {
  std::shared_ptr<service::SpriteManager> sprite_manager;
  std::shared_ptr<service::Time> time;
public:
  SpriteFrameAnimation(std::shared_ptr<service::SpriteManager> sprite_manager,
                       std::shared_ptr<service::Time> time):
    sprite_manager(std::move(sprite_manager)),
    time(std::move(time))
  {}
  void setup(Settings& settings) const override {
    settings.priority = update_priority::SpriteFrameAnimation;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Sprite, component::SpriteFrameAnimation>();
    float dtime = time->get_delta();
    view.each([dtime,this](auto entity, auto& sprite, auto& animation) {
      if (animation.icon_cache != sprite.icon || animation.state_cache != sprite.state) {
        animation.icon_cache = sprite.icon;
        animation.state_cache = sprite.state;
        sprite.frame = 0;
        sprite.dir = 0;
        const State* state = sprite_manager->get_sprite_data(animation.icon_cache, animation.state_cache);
        if (!state || state->delays.size() == 0) {
          animation.no_animation = true;
          return;
        }
        animation.delay = state->delays.front();
        return;
      }
      if (animation.no_animation) {
        return;
      }
      animation.delay -= dtime;
      if (animation.delay <= 0.0f) {
        const State* state = sprite_manager->get_sprite_data(animation.icon_cache, animation.state_cache);
        if (!state) {
          animation.no_animation = true;
          sprite.icon = -1;
          sprite.state = -1;
          return;
        }
        ++sprite.frame;
        if (sprite.frame >= state->delays.size()) {
          sprite.frame = 0;
        }
        if (sprite.frame < state->delays.size() && sprite.frame >= 0) {
          animation.delay = state->delays[sprite.frame];
        } else {
          animation.no_animation = true;
          sprite.icon = -1;
          sprite.state = -1;
        }
      }
    });
  }
};

CORE_DEFINE_SYSTEM("system::SpriteFrameAnimation", [](core::ServiceLocator& locator){
  return std::make_unique<SpriteFrameAnimation>(
    locator.get<service::SpriteManager>(),
    locator.get<service::Time>()
  );
});



}
