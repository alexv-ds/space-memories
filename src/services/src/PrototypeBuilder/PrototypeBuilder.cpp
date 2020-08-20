#include <core/LoggerFactory.hpp>
#include <services/PrototypeBuilder.hpp>
#include <core/define_service.hpp>
//#include <experimental/filesystem>
#include <iostream>
#include <components/Sprite.hpp>
#include "ComponentEmplacer.hpp"
#include "EmplacerBuilder.hpp"

namespace service {

component::Sprite build_Sprite() {
  return {};
}



class PrototypeBuilderImpl final : public PrototypeBuilder {
  std::shared_ptr<core::Logger> logger;
  core::ServiceLocator& locator;
public:
  PrototypeBuilderImpl(std::shared_ptr<core::Logger> new_logger,
                       core::ServiceLocator& locator);
  std::string_view impl_name() const noexcept override;
  entt::entity build(const std::string_view name, entt::registry& registry) override {
    return entt::null;
  }
};

CORE_DEFINE_SERVICE(PrototypeBuilder,
                   "service::PrototypeBuilder",
                   core::after<core::LoggerFactory>(), [](core::ServiceLocator& locator) {
  return std::make_shared<PrototypeBuilderImpl>(
    locator.get<core::LoggerFactory>()->create_logger("service::PrototypeBuilder"),
    locator
  );
});

std::string_view PrototypeBuilderImpl::impl_name() const noexcept {
  return "service::PrototypeBuilderImpl";
}

PrototypeBuilderImpl::PrototypeBuilderImpl(std::shared_ptr<core::Logger> new_logger,
                                           core::ServiceLocator& locator):
              logger(std::move(new_logger)),
              locator(locator)
{

  //logger->critical("omegalul");
}

}
