#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Input.hpp>
#include <components/Input.hpp>

namespace {

class InputServiceClear : public core::System {
  std::shared_ptr<service::Input> input_service;
  
public:
  InputServiceClear(std::shared_ptr<service::Input>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::InputServiceClear;
  }
  void update(entt::registry& registry) override {
    input_service->input_frame_end();
  }
};

InputServiceClear::InputServiceClear(std::shared_ptr<service::Input> input_service):
  input_service(std::move(input_service))
{}

CORE_DEFINE_SYSTEM("system::InputServiceClear", [](core::ServiceLocator& locator) {
  return std::make_unique<InputServiceClear>(
    std::move(locator.get<service::Input>())
  );
});

}