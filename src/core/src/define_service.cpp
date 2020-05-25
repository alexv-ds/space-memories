#include <core/define_service.hpp>

namespace core {

ServiceDefine::ServiceDefine(type_index interface,
                             std::vector<type_index> after,
                             std::string_view name,
                             BuildFunc build_func):
          //init
          interface(interface),
          after(std::move(after)),
          name(name),
          build_func(std::move(build_func))
{
  get_vector().push_back(this);
}

std::vector<ServiceDefine*>& ServiceDefine::get_vector() {
  static std::vector<ServiceDefine*> systems;
  return systems;
}

const std::vector<ServiceDefine*>& ServiceDefine::get_defined_services() {
  return get_vector();
}


}