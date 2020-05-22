#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <utility>
#include <core/type_index.hpp>
#include <core/Logger.hpp>
#include <core/Service.hpp>

namespace core {

class TypeRegistry : public Service {
public:
  inline TypeRegistry(std::shared_ptr<Logger> logger);

  std::pair<std::string_view, bool> get_name(type_index type) const;
  std::pair<type_index, bool> get_type(std::string_view name) const;
  void add_type(type_index type, std::string_view name);
private:
  std::shared_ptr<Logger> logger = nullptr;
  std::unordered_map<type_index, std::string> type_string_map;
  std::unordered_map<std::string_view, type_index> string_type_map;
};

//Inline methods
inline TypeRegistry::TypeRegistry(std::shared_ptr<Logger> logger): logger(logger) {}

}