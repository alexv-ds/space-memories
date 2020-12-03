#include <core/define_system.hpp>
#include <services/SpriteManager.hpp>

namespace {

class SpriteManagerUpdate final : public core::System {
  std::shared_ptr<service::SpriteManager> sprite_manager;
public:
  SpriteManagerUpdate(std::shared_ptr<service::SpriteManager> sprite_manager):
    sprite_manager(std::move(sprite_manager))
  {}

  void update(entt::registry&) override {
    sprite_manager->update();
  }
};

CORE_DEFINE_SYSTEM("system::SpriteManagerUpdate", [](core::ServiceLocator& locator) {
  return std::make_unique<SpriteManagerUpdate>(std::move(locator.get<service::SpriteManager>()));
});

}