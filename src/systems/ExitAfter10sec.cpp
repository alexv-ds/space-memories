#include <ctime>
#include <core/define_system.hpp>
#include <core/Process.hpp>
#include <core/LoggerFactory.hpp>
#include <components/ExitAfterNSec.hpp>
#include "system_update_priority.hpp"

namespace {

class ExitAfterNsec final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<core::Process> process;

  std::time_t last_time;
    
public:
  ExitAfterNsec(std::shared_ptr<core::Logger> logger, std::shared_ptr<core::Process> process) {
    this->logger = logger;
    this->process = process;
    std::time(&last_time);
  }
  void setup(Settings& settings) const override {
    settings.priority = update_priority::ExitAfter10sec;
  }
  void update(entt::registry& registry) override {
    time_t current_time;
    std::time(&current_time);
    time_t delta_t = current_time - last_time;
    last_time = current_time;
    if (delta_t > 0) {
      auto view = registry.view<component::ExitAfterNsec>();
      for (entt::entity entity : view) {
        registry.patch<component::ExitAfterNsec>(entity, [&, this](auto& exit_comp) {
          exit_comp.delay -= delta_t;
          if (exit_comp.delay <= 0) {
            process->exit(exit_comp.reason);
            registry.destroy(entity);
          }
          logger->info("Выход через: {}", exit_comp.delay);
        });
      }
    }
  };
};

CORE_DEFINE_SYSTEM("system::ExitAfterNsec", [](core::ServiceLocator& locator){
  std::shared_ptr logger_factory = locator.get<core::LoggerFactory>();
  std::shared_ptr process = locator.get<core::Process>();
  return std::make_unique<ExitAfterNsec>(
    std::move(logger_factory->create_logger("system::ExitAfterNsec")),
    std::move(process)
  );
});

}
