#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <services/SpriteManager.hpp>
#include <components/Sprite.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Sprite",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    nlohmann::json& j_icon = json["icon"];
    nlohmann::json& j_state = json["state"];
    std::string_view str_state;
    if (!j_icon.is_string()) {
      logger.warn("[component::Sprite] icon должен быть строкой");
      return component::Sprite();
    }
    if (!j_state.is_null()) {
      if (j_state.is_string()) {
        str_state = j_state.get<std::string_view>();
      } else {
        logger.warn("[component::Sprite] state должен быть строкой");
        return component::Sprite();
      }
    }
    std::shared_ptr sprite_manager = locator.get<service::SpriteManager>();
    return sprite_manager->load_sprite(j_icon.get<std::string_view>(), str_state);
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::SpriteFrameAnimation",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::SpriteFrameAnimation();
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::AddSpriteFrameeAnimationIfNeeded",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::AddSpriteFrameeAnimationIfNeeded();
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::SpaceIconInitialise",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    nlohmann::json& j_dmi = json["dmi"];
    if (!j_dmi.is_string()) {
      logger.warn("[component::SpaceIconInitialise] dmi должен быть строкой");
      return component::SpaceIconInitialise();
    }
    std::string str_dmi = std::move(j_dmi.get<std::string>());
    return component::SpaceIconInitialise({std::move(str_dmi)});
  }
);
