#include <core/define_system.hpp>
#include <services/Time.hpp>
#include <components/Input.hpp>
#include <components/Position.hpp>

namespace {

class WASDRawInputMovable final : public core::System {
  std::shared_ptr<service::Time> time;
  float speed = 6.0f;
  float step_size;
public:
  WASDRawInputMovable(std::shared_ptr<service::Time> time): time(std::move(time)) {}
  void update(entt::registry& registry) override {
    step_size = speed * time->get_delta();
    auto view = registry.view<component::WASDRawInputMovable, component::Position, component::ListenKeyboard>();
    view.each([&, this](auto entity, const auto& wasd_movable, auto position, const auto& keyboard) {
      if (registry.valid(wasd_movable.input_listener)) {
        bool need_update = false;
        if (keyboard.is_key_pressed(sf::Keyboard::W)) {
          position.y += step_size;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::A)) {
          position.x -= step_size;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::S)) {
          position.y -= step_size;
          need_update = true;
        }
        if (keyboard.is_key_pressed(sf::Keyboard::D)) {
          position.x += step_size;
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
  return std::make_unique<WASDRawInputMovable>(locator.get<service::Time>());
});

}