#include <services/PrototypeBuilder/define_component_builder.hpp>
#include <components/Movement.hpp>

PROTOTYPE_DEFINE_COMPONENT_BUILDER("component::DenseIntersectionControll",
  [](nlohmann::json json, core::ServiceLocator& locator, core::Logger& logger) {
    return component::DenseIntersectionControll();
  }
);
