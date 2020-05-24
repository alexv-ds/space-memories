//Перепиши это, уебок

#include <algorithm>
#include <exception>
#include <core/define_system.hpp>
#include "SystemRegistry.hpp"

namespace core {

SystemRegistry::SystemRegistry(std::shared_ptr<Logger> logger, 
                               std::shared_ptr<ServiceLocator> locator,
                               std::shared_ptr<entt::registry> registry):
        //init
        logger(logger),
        locator(locator),
        registry(registry)
{
  logger->info("Инициализация систем");
  const std::vector<SystemDefine*>& defined_systems = SystemDefine::get_defined_systems();
  std::vector<std::pair<int,std::unique_ptr<System>>> unsorted_systems;

  for (SystemDefine* system_define : defined_systems) {
    logger->trace("Инициализация {}", system_define->name);
    try {
      std::unique_ptr<System> system = system_define->build_func(*locator);
      if (!system) {
        logger->error(
          "Не удалось создать систему {}. Функция инициализации вернула nullptr", system_define->name
        );
        continue;
      }
      system_names.insert({system.get(), system_define->name});
      System::Settings settings;
      system->setup(settings);
      unsorted_systems.emplace_back(settings.priority,std::move(system));
    }
    catch(const std::exception& e) {
      logger->error("Не удалось инициализировать систему {}: {}", system_define->name, e.what());
    }
  } //for loop end
  logger->trace("Сортировка систем");
  std::sort(unsorted_systems.begin(), unsorted_systems.end(), [](const auto& a, const auto& b ){
    return a.first < b.first;
  });
  for (auto& pair : unsorted_systems) {
    systems.push_back(std::move(pair.second));
  }
  logger->info("Зарегистрировано систем: {}", systems.size());

  std::string priority_report = "Порядок вызова систем: ";
  for (const auto& system : systems) {
    priority_report+= "\n  ";
    priority_report+= system_names[system.get()];
  }
  logger->debug("{}", std::move(priority_report));

  for (const auto& system : systems) {
    system->init(*registry);
  }
}

void SystemRegistry::update() {
  for (std::unique_ptr<System>& system : systems) {
    system->update(*registry);
  }
}

}