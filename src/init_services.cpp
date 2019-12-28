#include "init.hpp"
#include <entt/locator/locator.hpp>
#include <core/service/Shutdown.hpp>

void init_services() {
  entt::service_locator<core::service::Shutdown>::set();
}
