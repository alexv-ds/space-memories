#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Camera.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::Camera",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::Camera camera;
    if (!json["size_x"].is_null()) {
      if (json["size_x"].is_number()) {
        camera.size_x = json["size_x"].get<float>();
      } else {
        logger.warn("[component::Camera] size_x должен быть числом");
      }
    }
    if (!json["size_y"].is_null()) {
      if (json["size_y"].is_number()) {
        camera.size_y = json["size_y"].get<float>();
      } else {
        logger.warn("[component::Camera] size_y должен быть числом");
      }
    }
    return camera;
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::CameraFixedUnitSize",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::CameraFixedUnitSize fixed_size;
    if (!json["x"].is_null()) {
      if (json["x"].is_number()) {
        fixed_size.x = json["x"].get<float>();
      } else {
        logger.warn("[component::CameraFixedUnitSize] x должен быть числом");
      }
    }
    if (!json["y"].is_null()) {
      if (json["y"].is_number()) {
        fixed_size.y = json["y"].get<float>();
      } else {
        logger.warn("[component::CameraFixedUnitSize] y должен быть числом");
      }
    }
    return fixed_size;
  }
);
