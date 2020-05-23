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

  logger->debug("Инициализация core::TypeRegistry");
  type_registry = std::make_shared<TypeRegistry>(
    std::move(logger_factory->create_logger("core::TypeRegistry"))
  );

  logger->info("Инициализация сервисов");
  logger->debug("Инициализация сore::ServiceLocatorImpl");
  std::shared_ptr service_locator_impl = std::make_shared<ServiceLocatorImpl>(
    type_registry, std::move(logger_factory->create_logger("core::ServiceLocatorImpl"))
  );
  service_locator = service_locator_impl;

  type_registry->add_type(type_id<LoggerFactory>(), "core::LoggerFactory");
  service_locator_impl->add_service(type_id<LoggerFactory>(), logger_factory);

  type_registry->add_type(type_id<TypeRegistry>(), "core::TypeRegistry");
  service_locator_impl->add_service(type_id<TypeRegistry>(), type_registry);

  logger->debug("Инициализация сore::ProcessImpl");

  std::shared_ptr<Process> process = std::make_shared<ProcessImpl>(
    std::move(logger_factory->create_logger("core::ProcessImpl")),
    [this](){this->exit();}, //fn_exin
    [this](){this->force_exit();} //fn_force_exit
  );
  type_registry->add_type(type_id<Process>(), "core::Process");
  service_locator_impl->add_service(type_id<Process>(), process);

  logger->info("Зарегестрированно сервисов: {}", service_locator_impl->service_count());
}

int Core::main() {
  RateLimiter rate_limiter(60);

  logger->debug("Инициализация entt::registry");
  std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

  logger->debug("Инициализация core::SystemRegistry");
  SystemRegistry system_registry(
    std::move(logger_factory->create_logger("core::SystemRegistry")),
    type_registry,
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