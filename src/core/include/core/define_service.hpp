#pragma once
//Сплошной говнокод, переписать это дерьмо

#include <functional>
#include <vector>
#include <string_view>
#include <type_traits>
#include <core/Service.hpp>
#include <core/ServiceLocator.hpp>
#include <core/define_helpers.hpp>

#define CORE_DEFINE_SERVICE(interface, name, require, lambda)\
  namespace {\
    const ::core::ServiceBuilder CORE_GENERATE_UNIQUE_NAME(service_builder) = \
      ::core::create_service_builder<interface>(name, std::move(require), std::move(lambda));\
  }

namespace core {

class ServiceBuilder;
void register_service_builder(ServiceBuilder* builder);
std::vector<ServiceBuilder*>& get_registered_service_builders();

class ServiceBuilder {
  using BuildFunc = std::function<std::shared_ptr<Service>(ServiceLocator&)>;
public:
  ServiceBuilder(type_index interface,
                        std::string_view name,
                        std::vector<type_index> require,
                        BuildFunc build_func):
            //init
            interface(interface),
            name(name),
            require(std::move(require)),
            build_func(build_func)
  {
    register_service_builder(this);
  }
  
  const type_index interface;
  const std::string name;
  const std::vector<type_index> require;
  const BuildFunc build_func;
};

template<class Interface, class BuildFunc>
const ServiceBuilder create_service_builder(std::string_view name,
                                            std::vector<type_index> require,
                                            BuildFunc lambda)
{
  std::function impl_builder = std::move(lambda);
  using ImplType = typename decltype(impl_builder)::result_type::element_type;
  static_assert(std::is_base_of_v<Service, Interface>);
  static_assert(std::is_base_of_v<Interface, ImplType>);
  static_assert(std::is_same_v<ServiceLocator&, typename decltype(impl_builder)::argument_type>);
  std::function<std::shared_ptr<Service>(ServiceLocator&)> service_builder = std::move(impl_builder);

  return ServiceBuilder(type_id<Interface>(), name, std::move(require), std::move(service_builder));
}

}