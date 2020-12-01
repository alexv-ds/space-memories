#pragma once

#include <memory>
#include <string>
#include <entt/entity/registry.hpp>
#include <entt/entity/observer.hpp>

namespace core {

class System {
public:
  virtual void update(entt::registry&);
  virtual void init(entt::registry&);
  virtual ~System() = default;
};

}