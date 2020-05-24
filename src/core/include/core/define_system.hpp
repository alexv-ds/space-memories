#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <type_traits>
#include <vector>
#include <core/type_index.hpp>
#include <core/define_helpers.hpp>
#include <core/System.hpp>
#include <core/ServiceLocator.hpp>

#define CORE_DEFINE_SYSTEM(name, lambda)\
  namespace {\
    const ::core::SystemDefine CORE_GENERATE_UNIQUE_NAME(system_define) =\
      ::core::define_system(name, lambda);\
  }

namespace core {

class SystemDefine {
  using BuildFunc = std::function<std::unique_ptr<System>(ServiceLocator&)>;
public:
  std::string name;
  BuildFunc build_func;

  static const std::vector<SystemDefine*>& get_defined_systems();
  SystemDefine(std::string_view name, BuildFunc build_func);
private:
  static std::vector<SystemDefine*>& get_vector();
  SystemDefine (const SystemDefine&) = delete;
  SystemDefine & operator = (const SystemDefine&) = delete;
};

template <class SystemBuildFunc>
SystemDefine define_system(std::string_view name, SystemBuildFunc lambda) {
  std::function impl_build_func = std::move(lambda);
  using SystemType = typename decltype(impl_build_func)::result_type::element_type;
  static_assert(std::is_base_of_v<System, SystemType>);
  static_assert(std::is_same_v<ServiceLocator&, typename decltype(impl_build_func)::argument_type>);
  return {name, std::move(impl_build_func)};
}

}