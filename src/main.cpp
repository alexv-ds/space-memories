#include <iostream>
#include <vector>
#include <exception>
#include <csignal>
#include <atomic>
#include <cstdlib>

#include <core/RateLimiter.hpp>
#include <core/service/Shutdown.hpp>
#include "init.hpp"

namespace {
  volatile std::atomic_bool g_sigint = false;
}

void sigint_handler(int) {
  bool old_value = g_sigint.exchange(true);
  if (old_value == true) {
    std::quick_exit(0);
  }
}

int main() {
  std::signal(SIGINT, sigint_handler);

  std::vector<SystemPtr> systems;
  entt::registry registry;
  init_services();
  init_systems(systems);

  for (SystemPtr& system : systems) {
    system->set_registry(&registry);
  }
  for (SystemPtr& system : systems) {
    system->init();
  }

  std::shared_ptr p_shutdown_service =
    core::service::get_service<core::service::Shutdown>();

  core::RateLimiter rate_limiter(60); //fps

  std::cout << "Loop start" << '\n';
  while (p_shutdown_service->on_close() == false && !g_sigint.load()) {
    rate_limiter.wait_next();
    for (SystemPtr& system : systems) {
      try {
        system->update();
      } catch (std::exception& e) {
        std::cout << "[exception from " << system->name();
        std::cout << "]: " << e.what() << std::endl;
        std::rethrow_exception(std::current_exception());
      }
    }
  }
  p_shutdown_service->graceful_close();
  std::cout << "Loop end" << '\n';
  systems.clear();
  destroy_services();
  return 0;
}
