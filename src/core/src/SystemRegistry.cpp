#include <algorithm>
#include <exception>
#include <fstream>
#include <chrono>
#include <core/define_system.hpp>
#include "SystemRegistry.hpp"

namespace core {
SystemRegistry::~SystemRegistry() = default;
SystemRegistry::SystemRegistry(std::shared_ptr<Logger> logger, 
                               std::shared_ptr<ServiceLocator> locator,
                               std::shared_ptr<entt::registry> registry,
                               std::shared_ptr<Process> process):
        //init
        logger(std::move(logger)),
        locator(std::move(locator)),
        registry(std::move(registry)),
        process(std::move(process))
{}

void SystemRegistry::update() {
  auto full_update_t1 = std::chrono::high_resolution_clock::now();
  for (SystemData& system_data : systems) {
    if (system_data.system) {
      auto t1 = std::chrono::high_resolution_clock::now();
      system_data.system->update(*registry);
      auto t2 = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> second_duraton = t2 - t1;
      system_data.current_execution_time = second_duraton.count();
    } else {
      system_data.current_execution_time = 0;
    }
  }
  for (SystemData& system_data : systems) {
    system_data.execution_time = system_data.current_execution_time;
  }
  auto full_update_t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> second_duraton = full_update_t2 - full_update_t1;
  update_time = second_duraton.count();
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

bool SystemRegistry::disable_system(SystemData& system_data) {
  logger->trace("Уничтожение {}", system_data.name);
  if (!system_data.system) {
    return false;
  }
  system_data.system = nullptr;
  return true;
}

void SystemRegistry::init_embeded_systems() {
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
  logger->info("Всего систем: {}", systems.size());
  logreport_unused_systems(priorities);
  
  sort_systems();
  for (SystemData& system_data : systems) {
    enable_system(system_data);
  } 
}

const std::vector<SystemRegistry::SystemData>& SystemRegistry::get_systems() const {
  return systems;
}

SystemRegistry::SystemData* SystemRegistry::find_system(std::string_view name) {
  for (SystemData& system_data : systems) {
    if (name == system_data.name) {
      return &system_data;
    }
  }
  return nullptr;
}

}