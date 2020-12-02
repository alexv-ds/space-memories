#include <core/define_system.hpp>
#include <components/Input.hpp>
#include <components/Position.hpp>

namespace {

class WASDRawInputMovable final : public core::System {
  float speed = 0.1f;
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::WASDRawInputMovable, component::Position, component::ListenKeyboard>();
    view.each([&, this](auto entity, const auto& wasd_movable, auto position, const auto& keyboard) {
      if (registry.valid(wasd_movable.input_listener)) {
        bool need_update = false;
        if (keyboard.is_key_pressed(sf::Keyboard::W)) {
          position.y += speed;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::A)) {
          position.x -= speed;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::S)) {
          position.y -= speed;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::D)) {
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
  return std::make_unique<WASDRawInputMovable>();
});

}