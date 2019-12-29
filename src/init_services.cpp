#include "init.hpp"
#include <entt/locator/locator.hpp>

#include <core/service/Shutdown.hpp>
#include <core/service/Window.hpp>

void init_services() {
  entt::service_locator<core::service::Shutdown>::set();
  entt::service_locator<core::service::Window>::set();
}

void destroy_services() {
  entt::service_locator<core::service::Shutdown>::reset();
  entt::service_locator<core::service::Window>::reset();
}
