#include <algorithm>
#include <exception>
#include <fstream>
#include <core/define_system.hpp>
#include "SystemRegistry.hpp"

namespace core {
SystemRegistry::~SystemRegistry() = default;
SystemRegistry::SystemRegistry(std::shared_ptr<Logger> logger, 
                               std::shared_ptr<ServiceLocator> locator,
                               std::shared_ptr<entt::registry> registry,
                               std::shared_ptr<Process> process):
        //init
        logger(logger),
        locator(locator),
        registry(registry),
        process(process)
{
  /*logger->info("Инициализация систем");
  const std::vector<SystemDefine*>& defined_systems = SystemDefine::get_defined_systems();
  std::vector<std::pair<int,std::unique_ptr<System>>> unsorted_systems;
  std::unordered_map<const System*, std::string> system_names;

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
  }*/
  logger->info("Инициализация систем");
  std::unordered_map<std::string, int> priorities = load_systems_priorities();
  for (SystemDefine* system_define : SystemDefine::get_defined_systems()) {
    SystemData system_data;
    system_data.name = system_define->name;
    system_data.factory = system_define->build_func;
    
    auto priorities_it = priorities.find(system_data.name);
    if (priorities_it == priorities.end()) {
      logger->error("{} не будет зарегестрирована, ее нет в списке приоритетов", system_data.name);
      continue;
    }
    system_data.priority = priorities_it->second;
    systems.push_back(std::move(system_data));
  }
  logreport_unused_systems(priorities);
  
  sort_systems();
  for (SystemData& system_data : systems) {
    enable_system(system_data);
  }
}

void SystemRegistry::update() {
  for (SystemData& system_data : systems) {
    if (system_data.system) {
      system_data.system->update(*registry);
    }
  }
}

std::unordered_map<std::string, int> SystemRegistry::load_systems_priorities() {
  std::unordered_map<std::string, int> priorities;
  try {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    file.open("systems-execute-priorities.txt");
    std::string line;
    int priority = 0;
    while(!file.eof() && std::getline(file, line)) {
      if (!line.size()) {
        continue;
      }
      auto [it, inserted] = priorities.insert({line, priority++});
      if (!inserted) {
        --priority;
        logger->warn("[systems-execute-priorities.txt] Дважды указана система {}", line);
      }
    }
  } catch (std::exception& e) {
    logger->critical("Не удалось загрузить приоритеты систем: {}", e.what());
    process->force_exit("Не удалось загрузить приоритеты систем");
  }
  return priorities;
}

void SystemRegistry::sort_systems() {
  std::sort(systems.begin(), systems.end(), [](const auto& a, const auto& b ){
    return a.priority < b.priority;
  });
}

void SystemRegistry::logreport_unused_systems(const std::unordered_map<std::string, int>& priorities) const {
  std::unordered_map<std::string, int> priorities_copy = priorities;
  for (const SystemData& system_data : systems) {
    priorities_copy.erase(system_data.name);
  }
  for (const auto& [name, priority] : priorities_copy) {
    logger->warn("{} - неизвестная система", name);
  }
}

bool SystemRegistry::enable_system(SystemData& system_data) {
  logger->trace("Инициализация {}", system_data.name);
  try {
    system_data.system = system_data.factory(*locator);
    if (!system_data.system) {
      throw std::logic_error("фабричная функция вернула nullptr");
    }
  } catch(const std::exception& e) {
    logger->error("Не удалось инициализировать систему {}: {}", system_data.name, e.what());
    return false;
  }
  system_data.system->init(*registry);
  return true;
}

}