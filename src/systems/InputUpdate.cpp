#include <core/define_system.hpp>
#include <components/Input.hpp>

namespace {

class InputUpdate : public core::System {
  std::vector<sf::Event> key_events;
  
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ListenWindowEvents, component::ListenKeyboard>();
    view.each([this, &registry](auto entity, const auto& events, const auto& keyboard) {
      key_events.clear();
      for(const sf::Event& event : events.events) {
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
          key_events.push_back(event);
        }
      }
      if (key_events.size()) {
        registry.patch<component::ListenKeyboard>(entity, [this](auto& keyboard) {
          for (const sf::Event& event : key_events) {
            if (event.type == sf::Event::KeyPressed) {
              keyboard.keys[event.key.code] = {false, true, true};
            } else {
              keyboard.keys[event.key.code] = {true, false, false};
            }
          }
        });
      }
    });
  }
};


CORE_DEFINE_SYSTEM("system::InputUpdate", [](core::ServiceLocator& locator) {
  return std::make_unique<InputUpdate>();
});

}