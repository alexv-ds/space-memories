#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <components/Position.hpp>
#include <components/RenderMode.hpp>
#include <services/SpriteManager.hpp>
#include <services/PrototypeBuilder.hpp>
#include <iostream>

namespace {

struct Turf {
  std::string_view icon;
  std::string_view state;
};

constexpr Turf floor_steel {"resources/floors.dmi", "steel"};
constexpr Turf floor_freezer {"resources/floors.dmi", "cult-narsie"};
constexpr Turf floor_showroom {"resources/floors.dmi", "showroomfloor"};
constexpr Turf wall_0 {"resources/walls.dmi", "0"};


std::vector<std::vector<Turf>> map {
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,wall_0,wall_0,wall_0,wall_0,wall_0,wall_0,wall_0,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_freezer,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,wall_0,wall_0,wall_0,wall_0,wall_0,wall_0,wall_0,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel},
  {floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel,floor_steel}
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
        entt::entity entity = registry.create();
        registry.emplace<component::Position>(entity, static_cast<float>(x), static_cast<float>(map.size() - y), 0.0f);
        registry.emplace<component::Body>(entity, 1.0f, 1.0f);
        registry.emplace<component::Sprite>(entity, sprite_manager->load_sprite(map[y][x].icon, map[y][x].state));
        registry.emplace<component::DefaultRenderMode>(entity);
        registry.emplace<component::SpriteFrameAnimation>(entity);
      }
    }

    prototype_builder->build("wall", registry);
  }
};

CORE_DEFINE_SYSTEM("system::MapInit", [](core::ServiceLocator& locator){
  return std::make_unique<MapInit>(
    locator.get<service::SpriteManager>(),
    locator.get<service::PrototypeBuilder>()
  );
});


}
