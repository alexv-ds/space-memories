#pragma once

#include <core/System.hpp>
#include <core/Logger.hpp>
#include <core/ServiceLocator.hpp>
#include <core/TypeRegistry.hpp>
#include <entt/entity/registry.hpp>

namespace core {

class SystemRegistry {
public:
  SystemRegistry(std::shared_ptr<Logger>, 
                 std::shared_ptr<TypeRegistry>,
                 std::shared_ptr<ServiceLocator>,
                 std::shared_ptr<entt::registry>);
  void update();
private:
  std::shared_ptr<Logger> logger;
  std::shared_ptr<TypeRegistry> type_registry;
  std::shared_ptr<ServiceLocator> locator;
  std::shared_ptr<entt::registry> registry;
  std::vector<std::unique_ptr<System>> systems;
};

}