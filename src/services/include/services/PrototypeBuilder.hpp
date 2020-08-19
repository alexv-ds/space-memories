#pragma once
#include <core/Service.hpp>
#include <entt/entity/registry.hpp>
#include <string_view>

namespace service {

class PrototypeBuilder : core::Service {
public:
  virtual entt::entity build(const std::string_view name, entt::registry& registry) = 0;
};

}
