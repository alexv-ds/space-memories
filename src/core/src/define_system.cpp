#include <core/define_system.hpp>

namespace core {

SystemDefine::SystemDefine(std::string_view name,
                           BuildFunc build_func):
  name(name), build_func(std::move(build_func))
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