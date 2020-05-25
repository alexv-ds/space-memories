#pragma once

#include <functional>
#include <vector>
#include <string_view>
#include <type_traits>
#include <core/Service.hpp>
#include <core/ServiceLocator.hpp>
#include <core/define_helpers.hpp>

#define CORE_DEFINE_SERVICE(interface, name, after, lambda)\
  namespace {\
    const ::core::ServiceDefine CORE_GENERATE_UNIQUE_NAME(service_define) =\
      ::core::define_service<interface>(name, std::move(after), std::move(lambda));\
  }

namespace core {

template<class... Args>
std::vector<type_index> after() {
  return {type_id<Args>()...}; 
}

class ServiceDefine {
  using BuildFunc = std::function<std::shared_ptr<Service>(ServiceLocator&)>;
public:
  type_index interface;
  std::vector<type_index> after;
  std::string name;
  BuildFunc build_func;

  static const std::vector<ServiceDefine*>& get_defined_services();
  ServiceDefine(type_index, std::vector<type_index>, std::string_view, BuildFunc);  
  ServiceDefine (const ServiceDefine&) = delete;
  ServiceDefine & operator = (const ServiceDefine&) = delete;
private:
  static std::vector<ServiceDefine*>& get_vector();
};

template <class InterfaceType, class ServiceBuildFunc>
ServiceDefine define_service(std::string_view name, std::vector<type_index> after, ServiceBuildFunc lambda) {
  std::function impl_build_func = std::move(lambda);
  using ImplType = typename decltype(impl_build_func)::result_type::element_type;
  static_assert(std::is_base_of_v<Service, InterfaceType>);
  static_assert(std::is_base_of_v<InterfaceType, ImplType>);
  std::function<std::shared_ptr<Service>(ServiceLocator&)> service_builder = std::move(impl_build_func);
  return {type_id<InterfaceType>(), std::move(after), name, service_builder};
}

}

