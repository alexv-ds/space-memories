#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <components/Input.hpp>
#include <components/RenderWindow.hpp>

namespace {

class CloseWindowEventHandler : public core::System { 
public:
  void setup(Settings& settings) const override {
    settings.priority = update_priority::CloseWindowEventHandler;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ListenWindowEvents, component::RenderWindow>();
    view.each([&registry](auto entity, const auto& events) {
      for (const sf::Event& event : events.events) {
        if (event.type == sf::Event::Closed) {
          registry.remove<component::RenderWindow>(entity);
          return;
        }
      }
    });
  }
};

CORE_DEFINE_SYSTEM("system::CloseWindowEventHandler", [](core::ServiceLocator& locator) {
  return std::make_unique<CloseWindowEventHandler>();
});

}