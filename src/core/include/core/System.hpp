#pragma once

#include <memory>
#include <string>
#include <entt/entity/registry.hpp>

namespace core {

class System {
public:
  virtual ~System() = default;
  virtual void update() = 0;
  virtual void setup(std::shared_ptr<entt::registry>);
protected:
  std::shared_ptr<entt::registry> registry;  
};

}