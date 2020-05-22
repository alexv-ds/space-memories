#include <cstdlib> //getenv
#include <core/Core.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/cfg/env.h>

class LoggerFactory final : public core::LoggerFactory {
  std::shared_ptr<spdlog::logger> logger = spdlog::default_logger(); 
public:
  std::shared_ptr<core::Logger> create_logger(std::string_view name) override {
    return logger->clone(std::string(name));
  }
};
int main(int argc, const char* const* argv) {
  spdlog::set_pattern("[%H:%M:%S][%^%l%$][%n]: %v");
#ifndef NDEBUG
  spdlog::set_level(spdlog::level::trace);  
#endif
  if (std::getenv("SPDLOG_LEVEL")) {
    spdlog::cfg::load_env_levels();
  }


  std::shared_ptr<LoggerFactory> logger_factory = std::make_shared<LoggerFactory>();
  std::shared_ptr<core::Logger> logger = std::move(logger_factory->create_logger("main"));
  logger->info("Инициализация core::Core");
  core::Core core(argc, argv, logger_factory);
  return core.main();
}