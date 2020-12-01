#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <services/PrototypeBuilder.hpp>
#include <components/Map.hpp>
#include <components/Position.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>

namespace {

/*
  Говнокод, сделанный на коленке, переписать,
  если не будет влом, конечно же
*/

class MapLoader : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::PrototypeBuilder> prototype_builder;
public:
  MapLoader(std::shared_ptr<core::Logger> logger,
            std::shared_ptr<service::PrototypeBuilder> prototype_builder):
    logger(std::move(logger)),
    prototype_builder(std::move(prototype_builder))
  {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::LoadMap>();
    //load_map передается не по ссылке потому что сразу удалим компонент
    //c энтити, но данные компонента еще понадобятся
    view.each([this, &registry](auto entity, const auto load_map_component) {
      registry.remove<component::LoadMap>(entity);
      logger->info("Загрузка карты: {}", load_map_component.file);
      auto time_begin = std::chrono::steady_clock::now();
      nlohmann::json j_map;
      if (!load_map(j_map, load_map_component.file)) {
        return;
      }
      nlohmann::json j_defines = j_map["defines"];
      std::map<char, std::vector<std::string>> defines;
      for (auto it = j_defines.begin(); it != j_defines.end(); ++it) {
        std::vector<std::string> prototypes;
        char key;
        if (it.key().size() != 1) {
          logger->warn(
            "Ключ должен быть 1 символ, \"{}\" - размером {}",
            it.key(),
            it.key().size()
          );
          continue;
        }
        key = it.key()[0];
        if (it->is_string()) {
          prototypes.push_back(std::move(it->get<std::string>()));
        } else if (it->is_array()) {
          for (nlohmann::json& elem : *it) {
            if (elem.is_string()) {
              prototypes.push_back(std::move(elem.get<std::string>()));
            } else {
              logger->warn("В ключе \"{}\" один из элементов не строка", key);
            }
          } //for end
        }
        defines.insert({key, std::move(prototypes)});
      } //for end
      size_t map_pos_y = j_map["map"].size();
      for (nlohmann::json& j_line : j_map["map"]) {
        --map_pos_y;
        size_t map_pos_x = 0;
        for (char ch : j_line.get<std::string>()) {
          auto defines_it = defines.find(ch);
          if (defines_it != defines.end()) {
            for (const std::string& prototype : defines_it->second) {
              entt::entity entity = prototype_builder->build(prototype, registry);
              float x = static_cast<float>(map_pos_x) + load_map_component.offset.x;
              float y = static_cast<float>(map_pos_y) + load_map_component.offset.y;
              if (entity != entt::null) {
                if (registry.has<component::Position>(entity)) {
                  registry.patch<component::Position>(entity, [x,y](auto& pos_component) {
                    pos_component.x = x;
                    pos_component.y = y;
                  });
                } else {
                  registry.emplace<component::Position>(entity, x, y);
                }
              }
            }//for end
          } else {
            logger->warn("Ключ \"{}\" по x: {}, y: {}, не найден", ch, map_pos_x, map_pos_y);
          }
          ++map_pos_x;
        }//for end
      }//for end
      auto time_end = std::chrono::steady_clock::now();
      std::chrono::duration<float, std::milli> elapsed_ms = time_end - time_begin;
      logger->info(
        "Загрузка карты {} завершена, время {:03.2f} мс", load_map_component.file, elapsed_ms.count()
      );
    });
  }
private:
  bool load_map(nlohmann::json& out, const std::string& f_name) {
    try {
      //код повторяется в PrototypeBuilder.cpp, забить хуй :)
      std::ifstream file;
      file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
      file.open(f_name, std::ios::in|std::ios::binary|std::ios::ate);
      size_t size = file.tellg();
      std::vector<char> buffer(size);
      file.seekg(0, std::ios::beg);
      file.read(buffer.data(), size);
      file.close();
      out = nlohmann::json::parse(buffer.begin(), buffer.end());
      if (!out.is_object()) {
        throw std::logic_error("json должен быть объектом");
      }
      if (!out["defines"].is_object()) {
        throw std::logic_error("defines должен быть объектом");
      }
      if (!out["map"].is_array()) {
        throw std::logic_error("map должен быть массивом");
      }
      for (const nlohmann::json& value : out["defines"]) {
        if (!(value.is_string() || value.is_array())) {
          throw std::logic_error("значения в defines должны быть строками и/или массивами");
        }
      }
      for (const nlohmann::json& value : out["map"]) {
        if (!value.is_string()) {
          throw std::logic_error("элементы в map должны быть строками");
        }
      }
    } catch(std::exception& e) {
      logger->error("Не удалось загрузить карту {}: {}", f_name, e.what());
      return false;
    }
    return true;
  }

};


CORE_DEFINE_SYSTEM("system::MapLoader", [](core::ServiceLocator& locator){
  return std::make_unique<MapLoader>(
    locator.get<core::LoggerFactory>()->create_logger("system::MapLoader"),
    locator.get<service::PrototypeBuilder>()
  );
});


}
