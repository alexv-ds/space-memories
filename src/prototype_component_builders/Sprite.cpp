#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Sprite.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Sprite",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    nlohmann::json& j_icon = json["icon"];
    nlohmann::json& j_state = json["state"];
    component::Sprite sprite;
    
    if (j_icon.is_string()) {
      sprite.icon = std::move(j_icon.get<std::string>());
    } else if (j_icon.is_null()) {
      sprite.icon = "";
    } else {
      logger.warn("[component::Sprite] icon должен быть строкой или null");
    }

    if (j_state.is_string()) {
      sprite.state = std::move(j_state.get<std::string>());
    } else if (j_state.is_null()) {
      sprite.state = "";
    } else {
      logger.warn("[component::Sprite] state должен быть строкой или null");
    }

    return sprite;
  }
);