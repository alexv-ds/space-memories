#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/RenderWindow.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::ExitIfWindowClosed",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::ExitIfWindowClosed();
  }
);

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::RenderWindow",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::RenderWindow();
  }
);
