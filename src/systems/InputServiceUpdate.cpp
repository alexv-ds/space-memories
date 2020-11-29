#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Input.hpp>
#include <components/Input.hpp>

namespace {

class InputServiceUpdate : public core::System {
  std::shared_ptr<service::Input> input_service;
  std::vector<sf::Event> event_buffer;
  
public:
  InputServiceUpdate(std::shared_ptr<service::Input>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::InputServiceUpdate;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ListenWindowEvents, component::ListenKeyboard>();
    view.each([this](auto entity, const auto& events) {
      for(const sf::Event& event : events.events) {
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
          event_buffer.push_back(event);
        }
      }
      if (event_buffer.size()) {
        input_service->update_keys(entity, event_buffer);
        event_buffer.clear();
      }
    });
  }
};

InputServiceUpdate::InputServiceUpdate(std::shared_ptr<service::Input> input_service):
  input_service(std::move(input_service))
{}

CORE_DEFINE_SYSTEM("system::InputServiceUpdate", [](core::ServiceLocator& locator) {
  return std::make_unique<InputServiceUpdate>(
    std::move(locator.get<service::Input>())
  );
});

}