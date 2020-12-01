#pragma once

#include <vector>
#include <unordered_map>
#include <core/System.hpp>
#include <core/Logger.hpp>
#include <core/Process.hpp>
#include <core/ServiceLocator.hpp>
#include <core/TypeRegistry.hpp>
#include <entt/entity/registry.hpp>

namespace core {

class SystemRegistry {
public:
  struct SystemData;
  SystemRegistry(std::shared_ptr<Logger>, 
                 std::shared_ptr<ServiceLocator>,
                 std::shared_ptr<entt::registry>,
                 std::shared_ptr<Process>);
  ~SystemRegistry();
  void update();
  void init_embeded_systems();
  const std::vector<SystemData>& get_systems() const;
  bool enable_system(SystemData&);
  bool disable_system(SystemData&);
  SystemData* find_system(std::string_view name);

private:
  std::shared_ptr<Logger> logger;
  std::shared_ptr<ServiceLocator> locator;
  std::shared_ptr<entt::registry> registry;
  std::shared_ptr<Process> process;

  std::vector<SystemData> systems;
  
  std::unordered_map<std::string, int> load_systems_priorities();
  void sort_systems();
  void logreport_unused_systems(const std::unordered_map<std::string, int>& priorities) const;
};

struct SystemRegistry::SystemData {
  using Factory = std::function<std::unique_ptr<System>(ServiceLocator&)>;
  std::unique_ptr<System> system;
  std::string name;
  int priority;
  Factory factory;
};

}