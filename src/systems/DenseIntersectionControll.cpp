#include <core/define_system.hpp>
#include <services/World.hpp>
#include <components/Position.hpp>
#include <components/Movement.hpp>
#include <components/ObjectTypes.hpp>
#include <vector>

namespace {

class DenseIntersectionControll final : public core::System {
  std::shared_ptr<service::World> world;
  std::vector<entt::entity> querry_buffer;
public:
  DenseIntersectionControll(std::shared_ptr<service::World> world): world(std::move(world)) {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Position,
                              component::Body,
                              component::DenseIntersectionControll,
                              component::DenseObject>();
    view.each([this, &registry](auto entity, auto position, const auto& body, auto intersect_contr) {
      querry_buffer.clear();
      world->query_intersects_region(querry_buffer, {position.x,position.y,body.size_x, body.size_y});
      for (entt::entity other_entity : querry_buffer) {
        if (entity == other_entity) {
          continue;
        }
        if (registry.has<component::DenseObject>(other_entity)) {
          position.x = intersect_contr.old_x;
          position.y = intersect_contr.old_y;
          registry.replace<component::Position>(entity, position);
          return;
        }
      }
      intersect_contr.old_x = position.x;
      intersect_contr.old_y = position.y;
      registry.replace<component::DenseIntersectionControll>(entity, intersect_contr);
    });
  }
};

CORE_DEFINE_SYSTEM("system::DenseIntersectionControll", [](core::ServiceLocator& locator){
  return std::make_unique<DenseIntersectionControll>(locator.get<service::World>());
});


}
