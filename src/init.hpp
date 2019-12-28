#pragma once

#include <core/ISystem.hpp>
#include <memory>
#include <vector>

using SystemPtr = std::unique_ptr<core::ISystem>;

void init_systems(std::vector<SystemPtr>& systems, entt::registry& registry);
void init_services();
