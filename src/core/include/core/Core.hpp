#pragma once

#include <core/Service.hpp>
#include <core/LoggerFactory.hpp>
#include <core/TypeRegistry.hpp>
#include <core/ServiceLocator.hpp>

namespace core {

class Core {
  std::shared_ptr<LoggerFactory> logger_factory;
  std::shared_ptr<Logger> logger;
  std::shared_ptr<TypeRegistry> type_registry;
  std::shared_ptr<ServiceLocator> service_locator;
public:
  Core(int argc, const char* const* argv, std::shared_ptr<LoggerFactory> logger_factory);
  int main();
};

}