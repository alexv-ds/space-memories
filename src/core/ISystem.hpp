#pragma once

#include <entt/entity/registry.hpp>
#include <string_view>

namespace core {

class ISystem {
public:

  virtual const std::string_view name() const noexcept = 0;
  virtual void update();
  virtual void init();

  void set_registry(entt::registry*) noexcept;
  virtual ~ISystem() = default;
  
protected:
  entt::registry* registry = nullptr;
};



} /* core */
