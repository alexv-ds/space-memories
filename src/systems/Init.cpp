#include "system_update_priority.hpp"

#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <core/Logger.hpp>
#include <services/Input.hpp>
#include <services/Time.hpp>
#include <components/RenderWindow.hpp>
#include <components/ExitAfterNSec.hpp>
#include <components/Input.hpp>

#include <SFML/Graphics.hpp>

namespace {

class Init final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Input> input;
  std::shared_ptr<service::Time> time;
  int counter = 360;
  bool delete_window = true;
public:
  Init(std::shared_ptr<core::Logger> logger,
       std::shared_ptr<service::Input> input,
       std::shared_ptr<service::Time> time):
    //init
    logger(logger),
    input(input),
    time(time)
  {}

  void setup(Settings& setting) const override {
    setting.priority = update_priority::Init;
  }
  void init(entt::registry& registry) override {
    entt::entity entity = registry.create();
    registry.emplace<component::RenderWindow>(entity);
    registry.emplace<component::ExitIfWindowClosed>(entity);
    registry.emplace<component::ListenKeyboard>(entity);
  }

  void update(entt::registry& registry) override {
    logger->debug("{}", time->get_delta());
  }

};

CORE_DEFINE_SYSTEM("system::Init", [](core::ServiceLocator& locator){
  return std::make_unique<Init>(
    locator.get<core::LoggerFactory>()->create_logger("system::Init"),
    locator.get<service::Input>(),
    locator.get<service::Time>()
  );
});


}