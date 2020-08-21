#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/RenderMode.hpp>

void process_blend_mode(component::DefaultRenderMode& mode, nlohmann::json& j_blend_mode, core::Logger& logger) {
  if(j_blend_mode.is_null()) {
    return;
  }
  if(!j_blend_mode.is_string()) {
    logger.warn("[component::DefaultRenderMode] blend_mode должен быть строкой");
    return;
  }
  std::string_view str_blend_mode = j_blend_mode.get<std::string_view>();
  if (str_blend_mode == "ADD") {
    mode.blend_mode = sf::BlendAdd;
  } else if (str_blend_mode == "MULTIPLY") {
    mode.blend_mode = sf::BlendMultiply;
  } else if (str_blend_mode == "ALPHA") {
    mode.blend_mode = sf::BlendAlpha;
  } else {
    mode.blend_mode = sf::BlendAlpha;
    logger.warn("[component::DefaultRenderMode] неизвестный blend_mode: {}", str_blend_mode);
  }
}

void process_color(component::DefaultRenderMode& mode, nlohmann::json& j_color, core::Logger& logger) {
  if (j_color.is_null()) {
    return;
  }
  if (!j_color.is_array()) {
    logger.warn("[component::DefaultRenderMode] color должен быть массивом");
    return;
  }
  size_t color_size = j_color.size();
  if (!(color_size == 3 || color_size == 4)) {
    logger.warn("[component::DefaultRenderMode] размер массива color должен быть 3 (rgb) или 4 (rgba)");
    return;
  }
  for (nlohmann::json& element : j_color) {
    if (!element.is_number()) {
      logger.warn("[component::DefaultRenderMode] элементы color должны быть числом");
      return;
    }
    int number = element.get<int>();
    if (number < 0 || number > 255) {
      logger.warn("[component::DefaultRenderMode] элементы color должны быть числом от 0 до 255");
      return;
    }
  }
  mode.color.r = static_cast<uint8_t>(j_color[0].get<int>());
  mode.color.g = static_cast<uint8_t>(j_color[1].get<int>());
  mode.color.b = static_cast<uint8_t>(j_color[2].get<int>());
  if (color_size == 4) {
    mode.color.a = static_cast<uint8_t>(j_color[3].get<int>());
  }
}

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::DefaultRenderMode",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    component::DefaultRenderMode mode;
    process_blend_mode(mode, json["blend_mode"], logger);
    process_color(mode, json["color"], logger);
    return mode;
  }
);
