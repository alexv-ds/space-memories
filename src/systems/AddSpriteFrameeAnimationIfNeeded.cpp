#include <core/define_system.hpp>
#include <components/Sprite.hpp>
#include <services/SpriteManager.hpp>

namespace {

class AddSpriteFrameeAnimationIfNeeded : public core::System {
  std::shared_ptr<service::SpriteManager> sprite_manager;
public:
  AddSpriteFrameeAnimationIfNeeded(std::shared_ptr<service::SpriteManager> sprite_manager):
    sprite_manager(std::move(sprite_manager))
  {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::AddSpriteFrameeAnimationIfNeeded>();
    view.each([this, &registry](auto entity) {
      registry.remove<component::AddSpriteFrameeAnimationIfNeeded>(entity);
      const component::Sprite* p_sprite = registry.try_get<component::Sprite>(entity);
      if (!p_sprite) {
        return;
      }
      const State* state = sprite_manager->get_sprite_data(p_sprite->icon, p_sprite->state);
      if (state && state->delays.size() > 0) {
       registry.emplace_or_replace<component::SpriteFrameAnimation>(entity); 
      }
    });
  }
};

CORE_DEFINE_SYSTEM("system::AddSpriteFrameeAnimationIfNeeded", [](core::ServiceLocator& locator){
  return std::make_unique<AddSpriteFrameeAnimationIfNeeded>(
    locator.get<service::SpriteManager>()
  );
});



}