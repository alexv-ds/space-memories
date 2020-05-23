#include <core/define_system.hpp>

namespace core {

SystemDefine::SystemDefine(std::string_view name,
                           type_index type,
                           std::vector<type_index> after,
                           BuildFunc build_func):
  name(name), type(type), after(std::move(after)), build_func(std::move(build_func))
{
  get_vector().push_back(this);
}

const std::vector<SystemDefine*>& SystemDefine::get_defined_systems() {
  return get_vector();
}

std::vector<SystemDefine*>& SystemDefine::get_vector() {
  static std::vector<SystemDefine*> systems;
  return systems;
}

};