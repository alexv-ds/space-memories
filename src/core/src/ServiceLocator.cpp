#include <stdexcept>
#include <core/ServiceLocator.hpp>

namespace core {

std::shared_ptr<Service> ServiceLocator::get(type_index type) {
  std::shared_ptr service = try_get(type);
  if (service) {
    return service;
  }
  auto [name, found] = type_registry->get_name(type);
  std::string throw_msg = "Сервис не найден: ";
  throw_msg += name;
  throw std::logic_error(throw_msg);
}

}