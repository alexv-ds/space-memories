#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Input.hpp>

namespace {

class WindowEventsHandlerClear : public core::System {
  std::shared_ptr<service::Input> input_service;
  
public:
  WindowEventsHandlerClear(std::shared_ptr<service::Input>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::WindowEventHandlerClear;
  }
  void update(entt::registry& registry) override {
    input_service->input_frame_end();
  }
};

WindowEventsHandlerClear::WindowEventsHandlerClear(std::shared_ptr<service::Input> input_service):
  input_service(std::move(input_service))
{}

CORE_DEFINE_SYSTEM("system::WindowEventsHandlerClear", [](core::ServiceLocator& locator) {
  return std::make_unique<WindowEventsHandlerClear>(
    std::move(locator.get<service::Input>())
  );
});

}