#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Map.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::LoadMap",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::LoadMap load_map;
    if (json["file"].is_string()) {
      load_map.file = std::move(json["file"].get<std::string>());
    } else {
      logger.warn("[component::LoadMap] file должен быть строкой");
    }
    nlohmann::json offset_array = json["offset"];
    if (offset_array.is_null()) {
      return load_map;
    }
    if (!offset_array.is_array()) {
      logger.warn("[component::LoadMap] offset должен быть массивом");
      return load_map;
    }
    if (offset_array.size() != 2) {
      logger.warn("[component::LoadMap] размер offset должен быть равен 2");
      return load_map;
    }
    if (!offset_array[0].is_number() || !offset_array[1].is_number()) {
      logger.warn("[component::LoadMap] offset должен быть массивом чисел");
      return load_map;
    }
    load_map.offset.x = offset_array[0].get<float>();
    load_map.offset.y = offset_array[1].get<float>();
    return load_map;
  }
);
