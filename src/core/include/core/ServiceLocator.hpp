#pragma once

#include <core/Service.hpp>
#include <core/TypeRegistry.hpp>
#include <core/type_index.hpp>

namespace core {

class ServiceLocator {
protected:
  std::shared_ptr<TypeRegistry> type_registry = nullptr;
public:
  inline ServiceLocator(std::shared_ptr<TypeRegistry>);
  virtual std::shared_ptr<Service> try_get(type_index type) noexcept = 0;
  virtual ~ServiceLocator() = default;
};

//inline methods
inline ServiceLocator::ServiceLocator(std::shared_ptr<TypeRegistry> type_registry):
      //initialise
      type_registry(type_registry) 
{};


};