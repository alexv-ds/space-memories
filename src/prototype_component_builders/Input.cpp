#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Input.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::ListenKeyboard",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::ListenKeyboard();
  }
);
