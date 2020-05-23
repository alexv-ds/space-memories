#pragma once

#include <atomic>
#include <core/Service.hpp>
#include <core/LoggerFactory.hpp>
#include <core/TypeRegistry.hpp>
#include <core/ServiceLocator.hpp>

namespace core {

class Core {
public:
  Core(int argc, const char* const* argv, std::shared_ptr<LoggerFactory> logger_factory);
  int main();

  //can be used in signal handlers
  void exit() volatile;
  void force_exit() volatile;
private:
  std::shared_ptr<LoggerFactory> logger_factory;
  std::shared_ptr<Logger> logger;
  std::shared_ptr<TypeRegistry> type_registry;
  std::shared_ptr<ServiceLocator> service_locator;
  volatile std::atomic_bool close_signal = false;
};

}