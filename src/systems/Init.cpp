#include "system_update_priority.hpp"

#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <core/Logger.hpp>

#include <components/RenderWindow.hpp>
#include <components/ExitAfterNSec.hpp>

namespace {

class Init final : public core::System {
  std::shared_ptr<core::Logger> logger;
public:
  Init(std::shared_ptr<core::Logger> logger): logger(logger) {};
  void setup(Settings& setting) const override {
    setting.priority = update_priority::Init;
  }
  void init(entt::registry& registry) override {
    entt::entity entity = registry.create();
    registry.emplace<component::RenderWindow>(entity);
    registry.emplace<component::ExitAfterNsec>(entity, "Тестинг", 7);
  }

};

CORE_DEFINE_SYSTEM("system::Init", [](core::ServiceLocator& locator){
  return std::make_unique<Init>(locator.get<core::LoggerFactory>()->create_logger("system::Init"));
});


}