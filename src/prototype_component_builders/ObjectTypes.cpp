#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/ObjectTypes.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::DenseObject",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::DenseObject();
  }
);
