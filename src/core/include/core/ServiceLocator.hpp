#pragma once

#include <exception>
#include <core/Service.hpp>
#include <core/TypeRegistry.hpp>
#include <core/type_index.hpp>

namespace core {

class ServiceLocator {
public:
  inline ServiceLocator() = default;
  virtual ~ServiceLocator() = default;
  virtual std::shared_ptr<Service> try_get(type_index type) noexcept = 0;
  std::shared_ptr<Service> get(type_index type);
  
  template<class Interface>
  std::shared_ptr<Interface> try_get() noexcept;

  template<class Interface>
  std::shared_ptr<Interface> get();

private:
  ServiceLocator (const ServiceLocator&) = delete;
  ServiceLocator& operator= (const ServiceLocator&) = delete;
};

//template methods
template<class Interface>
std::shared_ptr<Interface> ServiceLocator::try_get() noexcept {
  return std::dynamic_pointer_cast<Interface>(try_get(type_id<Interface>()));
}

template<class Interface>
std::shared_ptr<Interface> ServiceLocator::get() {
  std::shared_ptr service = std::dynamic_pointer_cast<Interface>(
    std::move( get(type_id<Interface>()) )
  );
  if (!service) {
    throw std::logic_error("dynamic_pointer_cast error");
  }
  return service;
}



};