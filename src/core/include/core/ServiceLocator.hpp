#pragma once

#include <exception>
#include <core/Service.hpp>
#include <core/TypeRegistry.hpp>
#include <core/type_index.hpp>

namespace core {

class ServiceLocator {
public:
  inline ServiceLocator(std::shared_ptr<TypeRegistry>);
  virtual ~ServiceLocator() = default;
  virtual std::shared_ptr<Service> try_get(type_index type) noexcept = 0;
  std::shared_ptr<Service> get(type_index type);
  
  template<class Interface>
  std::shared_ptr<Interface> try_get() noexcept;

  template<class Interface>
  std::shared_ptr<Interface> get();

protected:
  std::shared_ptr<TypeRegistry> type_registry = nullptr;
private:
  ServiceLocator (const ServiceLocator&) = delete;
  ServiceLocator& operator= (const ServiceLocator&) = delete;
};

//inline methods
inline ServiceLocator::ServiceLocator(std::shared_ptr<TypeRegistry> type_registry):
      //initialise
      type_registry(type_registry) 
{};

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
    auto [name, found] = type_registry->get_name(type_id<Interface>());
    std::string error_msg = "dynamic_pointer_cast<";
    error_msg += name;
    error_msg += "> error";
    throw std::logic_error(error_msg);
  }
  return service;
}



};