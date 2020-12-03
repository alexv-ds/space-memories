#include "SystemManagerImpl.hpp"

namespace core {
  
SystemManagerImpl::SystemManagerImpl(std::shared_ptr<Logger> logger,
                                     std::shared_ptr<SystemRegistry> system_registry):
  logger(std::move(logger)),
  system_registry(std::move(system_registry))
{}

std::string_view SystemManagerImpl::impl_name() const noexcept {
  return "core::SystemManagerImpl";
}

void SystemManagerImpl::each_system(std::function<void(const SystemStatus&)> cb) {
  for (const SystemRegistry::SystemData& system_data : system_registry->get_systems()) {
    system_status_cache.name = system_data.name;
    system_status_cache.priority = system_data.priority;
    system_status_cache.active = system_data.system != nullptr;
    system_status_cache.execution_time = system_data.execution_time;
    cb(system_status_cache);
  }
}

bool SystemManagerImpl::toggle_system(std::string_view name) {
  core::SystemRegistry::SystemData* system_data = system_registry->find_system(name);
  if (system_data->system) {
    return system_registry->disable_system(*system_data);
  } else {
    return system_registry->enable_system(*system_data);
  }
}

double SystemManagerImpl::get_systems_update_time() const {
  return system_registry->get_systems_update_time();
}

}