#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Input.hpp>
#include <components/Input.hpp>
#include <components/Position.hpp>


namespace {

class WASDRawInputMovable final : public core::System {
  std::shared_ptr<service::Input> input;
  float speed = 0.1f;
public:
  WASDRawInputMovable(std::shared_ptr<service::Input> input): input(input) {}

  void setup(Settings& settings) const override {
    settings.priority = update_priority::WASDRawInputMovable;
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::WASDRawInputMovable, component::Position>();
    view.each([&, this](auto entity, const auto& wasd_movable, auto position) {
      if (registry.valid(wasd_movable.input_listener)) {
        bool need_update = false;
        if (input->is_key_pressed(wasd_movable.input_listener, Key::W)) {
          position.y += speed;
          need_update = true;
        }
        if (input->is_key_pressed(wasd_movable.input_listener, Key::A)) {
          position.x -= speed;
          need_update = true;
        }
        if (input->is_key_pressed(wasd_movable.input_listener, Key::S)) {
          position.y -= speed;
          need_update = true;
        }
        if (input->is_key_pressed(wasd_movable.input_listener, Key::D)) {
          position.x += speed;
          need_update = true;
        }
        if (need_update) {
          registry.replace<component::Position>(entity, position);
        }
      }
      
    });
  }
};

CORE_DEFINE_SYSTEM("system::WASDRawInputMovable", [](core::ServiceLocator& locator) {
  return std::make_unique<WASDRawInputMovable>(std::move(locator.get<service::Input>()));
});

}