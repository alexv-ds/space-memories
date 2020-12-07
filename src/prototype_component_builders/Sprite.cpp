#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <services/SpriteManager.hpp>
#include <components/Sprite.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::SpriteOld",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    nlohmann::json& j_icon = json["icon"];
    nlohmann::json& j_state = json["state"];
    std::string_view str_state;
    if (!j_icon.is_string()) {
      logger.warn("[component::SpriteOld] icon должен быть строкой");
      return component::SpriteOld();
    }
    if (!j_state.is_null()) {
      if (j_state.is_string()) {
        str_state = j_state.get<std::string_view>();
      } else {
        logger.warn("[component::SpriteOld] state должен быть строкой");
        return component::SpriteOld();
      }
    }
    std::shared_ptr sprite_manager = locator.get<service::SpriteManager>();
    return sprite_manager->load_sprite(j_icon.get<std::string_view>(), str_state);
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::ForceSpriteFrame",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::ForceSpriteFrame force_frame;
    nlohmann::json& j_frame = json["frame"];
    if (j_frame.is_null()) {
      return force_frame;
    }
    if (!j_frame.is_number_integer()) {
      logger.warn("[component::ForceSpriteFrame] frame должен быть целым числом");
    }
    force_frame.frame = j_frame.get<int>();
    return force_frame;
  }
);
