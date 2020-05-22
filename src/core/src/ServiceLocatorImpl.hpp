#pragma once

#include <unordered_map>
#include <core/ServiceLocator.hpp>

namespace core {

class ServiceLocatorImpl final : public ServiceLocator {
public:
  ServiceLocatorImpl(std::shared_ptr<TypeRegistry> type_registry,
                     std::shared_ptr<Logger> logger);
  
  std::shared_ptr<Service> try_get(type_index type) noexcept override;
  void add_service(type_index type, std::shared_ptr<Service> service);
  inline size_t service_count() const noexcept;
  
private:
  std::shared_ptr<Logger> logger = nullptr;
  std::unordered_map<type_index, std::shared_ptr<Service>> services;      
};

inline size_t ServiceLocatorImpl::service_count() const noexcept {
  return services.size();
}

}