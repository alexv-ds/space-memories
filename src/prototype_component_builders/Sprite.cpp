#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <services/SpriteManager.hpp>
#include <components/Sprite.hpp>

namespace service {

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Sprite",
  [](const nlohmann::json& json, core::ServiceLocator& locator, core::Logger& logger) {
    std::shared_ptr sprite_manager = locator.get<service::SpriteManager>();
    return sprite_manager->load_sprite("resources/floors.dmi", "snow");
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::SpriteFrameAnimation",
  [](const nlohmann::json& json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::SpriteFrameAnimation();
  }
);

}
