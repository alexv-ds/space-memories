#pragma once

#include <memory>
#include <string>
#include <entt/entity/registry.hpp>

namespace core {

struct SystemSettings;

class System {
public:
  struct Settings;
  virtual void update(entt::registry&);
  virtual void init(entt::registry&);
  virtual void setup(Settings&) const;
  virtual ~System() = default;
};

struct System::Settings {
  int priority = 0;
};


}