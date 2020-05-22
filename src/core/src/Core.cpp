#include <core/Core.hpp>
#include <core/Service.hpp>
#include "ServiceLocatorImpl.hpp"

namespace core {

Core::Core(int argc, const char* const* argv, std::shared_ptr<LoggerFactory> logger_factory):
  logger_factory(logger_factory)
{
  logger = std::move(logger_factory->create_logger("core::Core"));

  logger->debug("Инициализация core::TypeRegistry");
  type_registry = std::make_shared<TypeRegistry>(
    std::move(logger_factory->create_logger("core::TypeRegistry"))
  );

  logger->debug("Инициализация сore::ServiceLocatorImpl");
  std::shared_ptr service_locator_impl = std::make_shared<ServiceLocatorImpl>(
    type_registry, std::move(logger_factory->create_logger("core::ServiceLocatorImpl"))
  );
  service_locator = service_locator_impl;

  type_registry->add_type(type_id<LoggerFactory>(), "core::LoggerFactory");
  service_locator_impl->add_service(type_id<LoggerFactory>(), logger_factory);

  type_registry->add_type(type_id<TypeRegistry>(), "core::TypeRegistry");
  service_locator_impl->add_service(type_id<TypeRegistry>(), type_registry);



}

int Core::main() {

  return 0;
};

}