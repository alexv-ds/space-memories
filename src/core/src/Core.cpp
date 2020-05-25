#include <cstdlib>
#include <core/Core.hpp>
#include <core/Service.hpp>
#include "ServiceLocatorImpl.hpp"
#include "RateLimiter.hpp"
#include "SystemRegistry.hpp"
#include "ProcessImpl.hpp"

namespace core {

Core::Core(int argc, const char* const* argv, std::shared_ptr<LoggerFactory> logger_factory):
  logger_factory(logger_factory)
{
  logger = std::move(logger_factory->create_logger("core::Core"));

  logger->info("Инициализация сервисов");
  std::shared_ptr service_locator_impl = std::make_shared<ServiceLocatorImpl>(
    std::move(logger_factory->create_logger("core::ServiceLocator"))
  );
  service_locator = service_locator_impl;

  std::shared_ptr<Process> process = std::make_shared<ProcessImpl>(
    std::move(logger_factory->create_logger("core::Process")),
    [this](){this->exit();}, //fn_exin
    [this](){this->force_exit();} //fn_force_exit
  );
  service_locator_impl->add_service("core::Process", type_id<Process>(), process);
  service_locator_impl->add_service("core::LoggerFactory",type_id<LoggerFactory>(), logger_factory);
  //Подгрузка остальных сервисов
  service_locator_impl->init_service_defines();
  logger->info("Зарегестрированно сервисов: {}", service_locator_impl->service_count());
}

int Core::main() {
  RateLimiter rate_limiter(60);

  logger->debug("Инициализация entt::registry");
  std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

  logger->debug("Инициализация core::SystemRegistry");
  SystemRegistry system_registry(
    std::move(logger_factory->create_logger("core::SystemRegistry")),
    service_locator,
    registry
  );

  logger->info("Начало главного цикла");
  while (!close_signal.load()) {
    rate_limiter.wait_next();
    system_registry.update();
  }
  logger->info("Конец главного цикла");
  return 0;
};

void Core::exit() volatile {
  close_signal.store(true);
}
void Core::force_exit() volatile {
  if (close_signal.load()) {
    std::quick_exit(0);
  } else {
    std::exit(0);
  }
}

}