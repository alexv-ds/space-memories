#include "init.hpp"

#include <core/system/WindowUpdate.hpp>
#include <ss13/system/Init.hpp>

#define INIT_SYSTEM(T) systems.emplace_back(new T())

void init_systems(std::vector<SystemPtr>& systems) {
  INIT_SYSTEM(ss13::system::Init);
  INIT_SYSTEM(core::system::WindowUpdate);
}
