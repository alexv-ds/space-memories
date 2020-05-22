#include <core/TypeRegistry.hpp>

namespace core {

std::pair<std::string_view, bool> TypeRegistry::get_name(type_index type) const {
  auto it = type_string_map.find(type);
  if (it == type_string_map.end()) {
    return {"***UNKNOWN TYPE***", false};
  }
  return {it->second, true}; 
}

std::pair<type_index, bool> TypeRegistry::get_type(std::string_view name) const {
  class __NullType {};
  const static type_index null_type = type_id<__NullType>();

  auto it = string_type_map.find(name);
  if (it == string_type_map.end()) {
    return {null_type, false};
  }
  return {it->second, true};
}

void TypeRegistry::add_type(type_index type, std::string_view name) {
  auto [name_by_type, name_found] = get_name(type);
  auto [type_by_name, type_found] = get_type(name);
  if (type_found) {
    if (type == type_by_name) {
      logger->warn("Двойная регистрация типа {}", name);
    } else {
      logger->error("Регистрация различных типов под одним именем {}", name);
    }
    return;
  }
  if (name_found) {
    logger->warn("Невозможно зарегестрировать тип {}, уже существует под именем {}", name, name_by_type);
    return;
  }

  auto [it, inserted] = type_string_map.insert({type, std::move(std::string(name))});
  string_type_map.insert({it->second, type});
  logger->trace("Зарегестрирован тип {}", name);
}

}