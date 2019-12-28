#pragma once

#include <entt/entity/registry.hpp>
#include <string_view>

namespace core {

class ISystem {
public:
  ISystem(entt::registry&) noexcept;
  virtual ~ISystem() = default;

  virtual void update() = 0;
  virtual const std::string_view name() = 0;

protected:
  entt::registry& registry;
};



} /* core */
