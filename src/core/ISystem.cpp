#include "ISystem.hpp"

namespace core {
//

void ISystem::update() {
  //do nothing
}

void ISystem::init() {
  //do nothing
}

void ISystem::set_registry(entt::registry* registry) noexcept {
  this->registry = registry;
}


} /* core */
