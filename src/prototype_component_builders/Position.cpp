#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Position.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Position",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::Position position;
    nlohmann::json& j_x = json["x"];
    nlohmann::json& j_y = json["y"];
    nlohmann::json& j_layer = json["layer"];
    if (!j_x.is_null()) {
      if (j_x.is_number()) {
        position.x = j_x.get<float>();
      } else {
        logger.warn("[component::Position] x должен быть числом");
      }
    }
    if (!j_y.is_null()) {
      if (j_y.is_number()) {
        position.y = j_y.get<float>();
      } else {
        logger.warn("[component::Position] y должен быть числом");
      }
    }
    if (!j_layer.is_null()) {
      if (j_layer.is_number()) {
        position.layer = j_layer.get<float>();
      } else {
        logger.warn("[component::Position] layer должен быть числом");
      }
    }
    return position;
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Body",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::Body body;
    nlohmann::json& j_size_x = json["size_x"];
    nlohmann::json& j_size_y = json["size_y"];
    if (!j_size_x.is_null()) {
      if (j_size_x.is_number()) {
        body.size_x = j_size_x.get<float>();
      } else {
        logger.warn("[component::Body] size_x должен быть числом");
      }
    }
    if (!j_size_y.is_null()) {
      if (j_size_y.is_number()) {
        body.size_y = j_size_y.get<float>();
      } else {
        logger.warn("[component::Body] size_y должен быть числом");
      }
    }
    return body;
  }
);
