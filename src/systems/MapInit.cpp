#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <components/Position.hpp>
#include <components/RenderMode.hpp>
#include <services/SpriteManager.hpp>
#include <services/PrototypeBuilder.hpp>
#include <iostream>

namespace {

std::vector<std::vector<std::string>> map {
  {"turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava"},
  {"turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava"},
  {"turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava"},
  {"turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava","turf/floor/lava"},
  {"turf/wall","turf/wall","turf/wall","turf/wall","turf/wall","turf/wall","turf/wall","turf/wall","turf/wall"},
  {"turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor"},
  {"turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor"},
  {"turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor"},
  {"turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor"},
  {"turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor","turf/floor"}
};



class MapInit : public core::System {
  std::shared_ptr<service::SpriteManager> sprite_manager;
  std::shared_ptr<service::PrototypeBuilder> prototype_builder;
public:
  MapInit(std::shared_ptr<service::SpriteManager> sprite_manager,
          std::shared_ptr<service::PrototypeBuilder> prototype_builder):
    sprite_manager(std::move(sprite_manager)),
    prototype_builder(std::move(prototype_builder))
  {}
  void setup(Settings& settings) const override {
    settings.priority = update_priority::MapInit;
  }
  void init(entt::registry& registry) override {
    for (size_t y = 0; y < map.size(); ++y) {
      for (size_t x = 0; x < map[y].size(); ++x) {
        entt::entity entity = prototype_builder->build(map[y][x], registry);
        if (entity != entt::null) {
          component::Position* p_pos = registry.try_get<component::Position>(entity);
          if (p_pos) {
            p_pos->x = static_cast<float>(x);
            p_pos->y = static_cast<float>(map.size() - y - 1);
          } else {
            registry.destroy(entity);
          }
        }

      }
    }
  }
};

CORE_DEFINE_SYSTEM("system::MapInit", [](core::ServiceLocator& locator){
  return std::make_unique<MapInit>(
    locator.get<service::SpriteManager>(),
    locator.get<service::PrototypeBuilder>()
  );
});


}
