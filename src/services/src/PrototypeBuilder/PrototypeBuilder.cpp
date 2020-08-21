#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/PrototypeBuilder.hpp>
#include <services/PrototypeBuilder/EmplacerBuilder.hpp>
#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <experimental/filesystem>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <chrono>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace service {

class PrototypeBuilderImpl final : public PrototypeBuilder {
  using UptrEmplacer = std::unique_ptr<IComponentEmplacer>;
  std::shared_ptr<core::Logger> logger;
  core::ServiceLocator& locator;
  std::unordered_map<std::string, std::unique_ptr<IEmplacerBuilder>> emplacer_builders;
  std::unordered_map<std::string, std::vector<UptrEmplacer>> component_emplacers;
  std::unordered_map<std::string, nlohmann::json> prototypes;

public:
  PrototypeBuilderImpl(std::shared_ptr<core::Logger> new_logger,
                       core::ServiceLocator& locator);
  std::string_view impl_name() const noexcept override;
  std::vector<std::string> get_file_list();
  std::vector<nlohmann::json> load_prototypes();
  entt::entity build(const std::string_view name, entt::registry& registry) override;
};

CORE_DEFINE_SERVICE(PrototypeBuilder,
                   "service::PrototypeBuilder",
                   core::after<core::LoggerFactory>(), [](core::ServiceLocator& locator) {
  return std::make_shared<PrototypeBuilderImpl>(
    locator.get<core::LoggerFactory>()->create_logger("service::PrototypeBuilder"),
    locator
  );
});

//methods

std::string_view PrototypeBuilderImpl::impl_name() const noexcept {
  return "service::PrototypeBuilderImpl";
}

std::vector<std::string> PrototypeBuilderImpl::get_file_list() {
  namespace fs = std::experimental::filesystem;
  logger->trace("Получение списка файлов");
  std::vector<std::string> files;
  try {
    for(auto& dir_entry : fs::recursive_directory_iterator("prototypes")) {
      const fs::path& path = dir_entry.path();
      files.push_back(std::move(path.string()));
    }
  } catch (std::exception& e) {
    logger->critical("Не удалось получить список файлов: {}", e.what());
    return {};
  }
  logger->trace("Файлов для загрузки: {}", files.size());
  return files;
}

std::vector<nlohmann::json> PrototypeBuilderImpl::load_prototypes() {
  const std::vector<std::string> files = get_file_list();
  std::vector<nlohmann::json> prototypes;
  for (const std::string& f_name : files) {
    auto time_begin = std::chrono::steady_clock::now();
    nlohmann::json loaded_json;
    size_t prototypes_count = 0;
    try {
      std::ifstream file;
      file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
      file.open(f_name, std::ios::in|std::ios::binary|std::ios::ate);
      size_t size = file.tellg();
      std::vector<char> buffer(size);
      file.seekg(0, std::ios::beg);
      file.read(buffer.data(), size);
      file.close();
      loaded_json = nlohmann::json::parse(buffer.begin(), buffer.end());
      if (!(loaded_json.is_array() || loaded_json.is_object())) {
        throw std::logic_error("должен быть объектом или массивом");
      }
    } catch (std::exception& e) {
      logger->error("Не удалось загрузить {}: {}", f_name, e.what());
      continue;
    }
    if (loaded_json.is_object() && loaded_json["name"].is_string()) {
      prototypes.push_back(std::move(loaded_json));
      ++prototypes_count;
    } else if (loaded_json.is_array()) {
      for (nlohmann::json& prototype : loaded_json) {
        if (prototype.is_object() && prototype["name"].is_string()) {
          prototypes.push_back(std::move(prototype));
          ++prototypes_count;
        }
      }
    }
    auto time_end = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::milli> elapsed_ms = time_end - time_begin;
    logger->info("Загружен {}, время {:03.2f} мс, кол-во прототипов {}", f_name, elapsed_ms.count(), prototypes_count);
  }
  return prototypes;
}

PrototypeBuilderImpl::PrototypeBuilderImpl(std::shared_ptr<core::Logger> new_logger,
                                           core::ServiceLocator& locator):
              logger(std::move(new_logger)),
              locator(locator)
{
  std::vector<nlohmann::json> vec_prototypes = load_prototypes();
  prototypes.reserve(vec_prototypes.size());
  logger->info("Всего прототипов: {}", vec_prototypes.size());
  std::sort(vec_prototypes.begin(), vec_prototypes.end(), [](const nlohmann::json& lhs, const nlohmann::json& rhs) {
    std::string_view lhs_name = lhs["name"].get<std::string_view>();
    std::string_view rhs_name = rhs["name"].get<std::string_view>();
    return std::count(lhs_name.begin(), lhs_name.end(), '/') < std::count(rhs_name.begin(), rhs_name.end(), '/');
  });
  for (nlohmann::json& prototype : vec_prototypes) {
    std::string full_name = prototype["name"].get<std::string>();
    size_t last_slash_pos = full_name.find_last_of('/');
    if (last_slash_pos != std::string_view::npos) {
      std::string base_prototype_name = full_name.substr(0, last_slash_pos);
      auto base_json_it = prototypes.find(base_prototype_name);
      if (base_json_it != prototypes.end()) {
        nlohmann::json patch_json = std::move(prototype);
        prototype = base_json_it->second;
        prototype.merge_patch(std::move(patch_json));
      } else {
        logger->error(
          "Не удалось зарегестрировать прототип {}: не найден базовый прототип {}",
          full_name, base_prototype_name
        );
        continue;
      }
    }
    auto [insert_it, is_inserted] = prototypes.insert({full_name, std::move(prototype)});
    if (!is_inserted) {
      logger->error("Не удалось зарегестрировать прототип {}: имя занято", full_name);
    }
  }//loop end
  //загрузка component builders
  const std::vector<ComponentBuilderDefine*> defines =  ComponentBuilderDefine::get_defined_builders();
  std::unordered_set<core::type_index> registered_types;
  registered_types.reserve(defines.size());
  for (const ComponentBuilderDefine* define : defines) {
    std::unique_ptr<IEmplacerBuilder> emplacer_builder = define->f_emplacer_builder();
    auto [insert_type_id, is_inderted_type] = registered_types.insert(emplacer_builder->component_type());
    if (!is_inderted_type) {
      logger->error(
        "Не далось зарегестрировать сборщик коммпонента {}, тип ({}) уже занят",
        define->name, emplacer_builder->component_type().name()
      );
      continue;
    }
    auto [insert_emplacer_it, is_inserted_emplacer] =
      emplacer_builders.insert({define->name, std::move(emplacer_builder)});
    if (!is_inserted_emplacer) {
      logger->error("Не далось зарегестрировать сборщик коммпонента {}, имя занято", define->name);
      continue;
    }
  } //loop end
  component_emplacers.reserve(prototypes.size());
}

entt::entity PrototypeBuilderImpl::build(const std::string_view name, entt::registry& registry) {
  std::string str_name(name.begin(), name.end());
  auto component_emplacers_it = component_emplacers.find(str_name);
  if (component_emplacers_it != component_emplacers.end()) {
    entt::entity entity = registry.create();
    for (std::unique_ptr<IComponentEmplacer>& empalcer : component_emplacers_it->second) {
      empalcer->emplace(entity, registry);
    }
    return entity;
  }
  auto prototype_it = prototypes.find(str_name);
  if (prototype_it == prototypes.end()) {
    logger->error("Не удалось создать неизвестный прототип {}", name);
    return entt::null;
  }
  std::vector<UptrEmplacer> emplacers;
  logger->debug("Сборка прототипа: {}", name);
  for (auto j_it = prototype_it->second.begin(); j_it != prototype_it->second.end(); ++j_it) {
    if (j_it.key() == "name") {
      continue;
    }
    if (!j_it->is_object()) {
      logger->warn(
        "Компонент {} для {} не будет создан: прототип компонента должен быть объектом",
        j_it.key(), name
      );
      continue;
    }
    auto emplacer_builder_it = emplacer_builders.find(j_it.key());
    if (emplacer_builder_it == emplacer_builders.end()) {
      logger->warn(
        "Компонент {} для {} не будет создан: не найден сборщик компонента",
        j_it.key(), name
      );
      continue;
    }
    try {
      emplacers.push_back(std::move(emplacer_builder_it->second->build(*j_it, locator, *logger)));
    } catch (std::exception& e) {
      logger->warn(
        "Компонент {} для {} не будет создан, исключение при создании IComponentEmplacer: {}",
        j_it.key(), name, e.what()
      );
    }
  } //loop end
  if (emplacers.size() == 0) {
    logger->warn("Прототип {} не содержит в себе компонентов", name);
  }

  entt::entity entity = registry.create();
  for (std::unique_ptr<IComponentEmplacer>& empalcer : emplacers) {
    empalcer->emplace(entity, registry);
  }
  component_emplacers.insert({str_name, std::move(emplacers)});
  return entity;
}

}
