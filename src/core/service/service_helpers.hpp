#pragma once

#include <stdexcept>
#include <string>
#include <entt/locator/locator.hpp>

namespace core::service {

template <typename T>
std::shared_ptr<T> get_service() {
  std::weak_ptr weak_service = entt::service_locator<T>::get();
  std::shared_ptr shared_service = weak_service.lock();
  if (!shared_service) {
    std::string error_msg = "Failed to get '";
    error_msg += T::name;
    error_msg += "' service";
    throw std::runtime_error(error_msg);
  }
  return shared_service;
}

} /* core::service */
