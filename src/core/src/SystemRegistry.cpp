#include <exception>
#include <core/define_system.hpp>
#include "SystemRegistry.hpp"

namespace core {

SystemRegistry::SystemRegistry(std::shared_ptr<Logger> logger, 
                               std::shared_ptr<TypeRegistry> type_registry,
                               std::shared_ptr<ServiceLocator> locator,
                               std::shared_ptr<entt::registry> registry):
        //init
        logger(logger),
        type_registry(type_registry),
        locator(locator),
        registry(registry)
{
  logger->info("Инициализация систем");
  const std::vector<SystemDefine*>& defined_systems = SystemDefine::get_defined_systems();

  std::string system_update_order_msg;

  for (SystemDefine* system_define : defined_systems) {
    logger->trace("Инициализация {}", system_define->name);
    type_registry->add_type(system_define->type, system_define->name);
    try {
      std::unique_ptr<System> system = system_define->build_func(*locator);
      system->setup(registry);
      systems.push_back(std::move(system));
      system_update_order_msg += "\n  ";
      system_update_order_msg += system_define->name;
    }
    catch(const std::exception& e) {
      logger->error("Не удалось инициализировать систему {}: {}", system_define->name, e.what());
    }
  }
  if (system_update_order_msg.size()) {
    logger->debug("Порядок обновления систем:{}", system_update_order_msg);
  }
  logger->info("Зарегистрировано систем: {}", systems.size());
}

void SystemRegistry::update() {
  for (std::unique_ptr<System>& system : systems) {
    system->update();
  }
}

}