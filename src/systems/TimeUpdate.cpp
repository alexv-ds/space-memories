#include <core/define_system.hpp>
#include <services/Time.hpp>

namespace {

class TimeUpdate final : public core::System {
  std::shared_ptr<service::Time> time;
public:
  TimeUpdate(std::shared_ptr<service::Time> time): time(std::move(time)) {}

  void update(entt::registry&) override {
    time->update();
  }
};

CORE_DEFINE_SYSTEM("system::TimeUpdate", [](core::ServiceLocator& locator) {
  return std::make_unique<TimeUpdate>(std::move(locator.get<service::Time>()));
});

}