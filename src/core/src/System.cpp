#include <cassert>
#include <core/System.hpp>

namespace core {

void System::setup(std::shared_ptr<entt::registry> registry) {
  assert(registry != nullptr);
  this->registry = registry;
}

}