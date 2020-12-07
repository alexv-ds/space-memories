#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Other.hpp>

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
