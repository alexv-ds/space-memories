#include <core/define_system.hpp>
#include <services/World.hpp>

namespace {

class WorldUpdate final : public core::System {
  std::shared_ptr<service::World> world;
  entt::registry* p_registry = nullptr;
public:
  WorldUpdate(std::shared_ptr<service::World> world): world(world) {}
  ~WorldUpdate() {
    if (p_registry) {
      p_registry->on_construct<component::Position>().disconnect<&WorldUpdate::on_construct_cb>(this);
      p_registry->on_construct<component::Body>().disconnect<&WorldUpdate::on_construct_cb>(this);
      p_registry->on_destroy<component::Position>().disconnect<&WorldUpdate::on_destroy_cb>(this);
      p_registry->on_destroy<component::Body>().disconnect<&WorldUpdate::on_destroy_cb>(this);

      p_registry->on_update<component::Position>().disconnect<&service::World::update_entity>(world.get());
      p_registry->on_update<component::Body>().disconnect<&service::World::update_entity>(world.get());
    }
  }

  void init(entt::registry& registry) override {
    registry.on_construct<component::Position>().connect<&WorldUpdate::on_construct_cb>(this);
    registry.on_construct<component::Body>().connect<&WorldUpdate::on_construct_cb>(this);
    registry.on_destroy<component::Position>().connect<&WorldUpdate::on_destroy_cb>(this);
    registry.on_destroy<component::Body>().connect<&WorldUpdate::on_destroy_cb>(this);

    registry.on_update<component::Position>().connect<&service::World::update_entity>(world.get());
    registry.on_update<component::Body>().connect<&service::World::update_entity>(world.get());

    p_registry = &registry;
  }

  void on_construct_cb(entt::registry& registry, entt::entity entity) {
    if (registry.has<component::Position, component::Body>(entity)) {
      world->create_body(registry, entity);
    }
  }
  void on_destroy_cb(entt::registry& registry, entt::entity entity) {
    if (registry.has<component::Position, component::Body>(entity)){
      world->destroy_body(registry, entity);
    }
  }
};

CORE_DEFINE_SYSTEM("system::WorldUpdate", [](core::ServiceLocator& locator){
  return std::make_unique<WorldUpdate>(locator.get<service::World>());
});




}