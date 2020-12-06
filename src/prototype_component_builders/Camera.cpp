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

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::OverrideRenderRegion",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    nlohmann::json& j_rect = json["rect"];
    if (!j_rect.is_object()) {
      logger.warn("[component::OverrideRenderRegion] rect должен быть объектом");
      return component::OverrideRenderRegion();
    }
    nlohmann::json& j_left = j_rect["left"];
    nlohmann::json& j_top = j_rect["top"];
    nlohmann::json& j_width = j_rect["width"];
    nlohmann::json& j_height = j_rect["height"];
    
    component::OverrideRenderRegion region;
    if (!j_left.is_number()) {
      logger.warn("[component::OverrideRenderRegion] rect.left должен быть числом");
    } else {
      region.rect.left = j_left.get<float>();
    }
    if (!j_top.is_number()) {
      logger.warn("[component::OverrideRenderRegion] rect.top должен быть числом");
    } else {
      region.rect.top = j_top.get<float>();
    }
    if (!j_width.is_number()) {
      logger.warn("[component::OverrideRenderRegion] rect.width должен быть числом");
    } else {
      region.rect.width = j_width.get<float>();
    }
    if (!j_height.is_number()) {
      logger.warn("[component::OverrideRenderRegion] rect.height должен быть числом");
    } else {
      region.rect.height = j_height.get<float>();
    }
    return region;
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::DebugRenderRegionFrame",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::DebugRenderRegionFrame();
  }
);

