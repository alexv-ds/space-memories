#include <core/define_system.hpp>
#include <components/Sprite.hpp>
#include <components/Position.hpp>
#include <services/SpriteManager.hpp>
#include <cmath>

namespace {

class SpaceIconInitialise : public core::System {
  std::shared_ptr<service::SpriteManager> sprite_manager;
public:
  SpaceIconInitialise(std::shared_ptr<service::SpriteManager> sprite_manager):
    sprite_manager(std::move(sprite_manager))
  {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::SpaceIconInitialise>();
    //space_initialise не по константной ссылке потому что муваем строку
    //а компонент все равно будет удален, с соответствующим триггером
    view.each([this, &registry](auto entity, auto& space_initialise) {
      std::string dmi_name = std::move(space_initialise.dmi);
      registry.remove<component::SpaceIconInitialise>(entity);
      const component::Position* p_pos = registry.try_get<component::Position>(entity);
      if (!p_pos) {
        return;
      }
      int x = std::abs(static_cast<int>(p_pos->x));
      int y = std::abs(static_cast<int>(p_pos->y));
      int icon_state = std::abs((x + y) ^ ~(x * y)) % 25 + 1;
      registry.emplace_or_replace<component::Sprite>(
        entity, sprite_manager->load_sprite(dmi_name, std::to_string(icon_state))
      );
      
    });
  }
};

CORE_DEFINE_SYSTEM("system::SpaceIconInitialise", [](core::ServiceLocator& locator){
  return std::make_unique<SpaceIconInitialise>(
    locator.get<service::SpriteManager>()
  );
});
  
}