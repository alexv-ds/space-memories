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
  class SystemData;
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
  inline double get_systems_update_time() const;

private:
  std::shared_ptr<Logger> logger;
  std::shared_ptr<ServiceLocator> locator;
  std::shared_ptr<entt::registry> registry;
  std::shared_ptr<Process> process;

  std::vector<SystemData> systems;
  double update_time = 0;
  
  std::unordered_map<std::string, int> load_systems_priorities();
  void sort_systems();
  void logreport_unused_systems(const std::unordered_map<std::string, int>& priorities) const;
};

class SystemRegistry::SystemData {
public:
  using Factory = std::function<std::unique_ptr<System>(ServiceLocator&)>;
  
  std::unique_ptr<System> system;
  std::string name;
  int priority;
  Factory factory;
  double execution_time = 0; //время выполнения с прошлого цикла, изменяется, после отработки всех систем
private:
  friend class SystemRegistry; 
  double current_execution_time = 0; //Время выполнения на этом цикле
};

inline double SystemRegistry::get_systems_update_time() const {
  return update_time;
}

}