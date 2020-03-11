#include <core/ServiceLocator.hpp>
#include <core/service/LoggerFactory.hpp>

std::shared_ptr<core::ILogger> init_logger_factory(core::ServiceLocator& locator) {
  std::shared_ptr p_logger_factory = std::make_shared<core::service::LoggerFactory>();
  core::ServiceLocator service_locator;
  service_locator.set<core::service::LoggerFactory>(p_logger_factory);
  return p_logger_factory->create_logger("main");
}

int main(int argc, char const *argv[]) {
  core::ServiceLocator service_locator;
  std::shared_ptr p_main_logger = init_logger_factory(service_locator);
  

  return 0;
}
