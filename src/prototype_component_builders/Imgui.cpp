#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Imgui.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::ImguiDebugMenu",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::ImguiDebugMenu();
  }
);