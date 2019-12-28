#include "init.hpp"

#include <core/system/Window.hpp>

void init_systems(std::vector<SystemPtr>& systems, entt::registry& registry) {
  systems.emplace_back(new core::system::Window(registry));
}
