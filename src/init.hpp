#pragma once

#include <core/ISystem.hpp>
#include <memory>
#include <vector>

using SystemPtr = std::unique_ptr<core::ISystem>;

void init_systems(std::vector<SystemPtr>& systems);
void init_services();
void destroy_services();
