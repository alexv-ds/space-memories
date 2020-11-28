#include <cstdlib> //getenv
#include <csignal>
#include <ctime>
#include <core/Core.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/cfg/env.h>
#include "LoggerFactory.hpp"

#ifdef _WIN32
  #include <windows.h>
#endif

namespace {
  volatile core::Core* g_core = nullptr;
}

extern "C" void signal_handler(int sig) {
  if (g_core) {
    g_core->exit();
  } else {
    std::quick_exit(0);
  }
}

int main(int argc, const char* const* argv) {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif
  spdlog::set_pattern("[%H:%M:%S][%^%l%$][%n]: %v");
#ifndef NDEBUG
  spdlog::set_level(spdlog::level::trace);  
#endif
  if (std::getenv("SPDLOG_LEVEL")) {
    spdlog::cfg::load_env_levels();
  }

  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);

  std::srand(std::time(nullptr));

  std::shared_ptr<LoggerFactory> logger_factory = std::make_shared<LoggerFactory>();
  std::shared_ptr<core::Logger> logger = std::move(logger_factory->create_logger("main"));
  logger->debug("Инициализация core::Core");
  core::Core core(argc, argv, logger_factory);
  g_core = &core;
  int core_return_core = core.main();
  g_core = nullptr;
  return core_return_core;
}