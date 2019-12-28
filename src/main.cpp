#include <iostream>
#include <vector>
#include <exception>

#include <core/RateLimiter.hpp>
#include <core/service/Shutdown.hpp>
#include "init.hpp"

int main() {
  std::vector<SystemPtr> systems;
  entt::registry registry;
  init_services();
  init_systems(systems, registry);

  std::shared_ptr p_shutdown_service =
    core::service::get_service<core::service::Shutdown>();

  core::RateLimiter rate_limiter(5); //fps
  
  std::cout << "Loop start" << '\n';
  while (p_shutdown_service->on_close() == false) {
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
  std::cout << "Loop end" << '\n';
  systems.clear();
  return 0;
}
