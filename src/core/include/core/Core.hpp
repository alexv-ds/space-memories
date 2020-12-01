#pragma once

#include <atomic>
#include <core/Service.hpp>
#include <core/LoggerFactory.hpp>
#include <entt/entity/registry.hpp>

namespace core {

class TypeRegistry;
class ServiceLocator;
class SystemRegistry;

class Core {
public:
  Core(int argc, const char* const* argv, std::shared_ptr<LoggerFactory> logger_factory);
  ~Core();
  int main();

  //can be used in signal handlers
  void exit() volatile;
  void force_exit() volatile;
private:
  std::shared_ptr<LoggerFactory> logger_factory;
  std::shared_ptr<Logger> logger;
  std::shared_ptr<TypeRegistry> type_registry;
  std::shared_ptr<ServiceLocator> service_locator;
  std::shared_ptr<SystemRegistry> system_registry;
  std::shared_ptr<entt::registry> registry;
  volatile std::atomic_bool close_signal = false;
};

}