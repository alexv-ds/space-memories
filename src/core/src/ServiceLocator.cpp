#include <stdexcept>
#include <core/ServiceLocator.hpp>

namespace core {

std::shared_ptr<Service> ServiceLocator::get(type_index type) {
  std::shared_ptr service = try_get(type);
  if (service) {
    return service;
  }
  std::string throw_msg = "запрашиваемый сервис не найден";
  throw std::logic_error(throw_msg);
}

}