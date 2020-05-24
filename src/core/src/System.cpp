#include <cassert>
#include <core/System.hpp>

namespace core {

void System::update(entt::registry&) {}
void System::init(entt::registry&) {}
void System::setup(Settings&) const {}

}