#pragma once

#include <core/ISystem.hpp>
#include <core/service/Shutdown.hpp>

namespace core::system {

class Window final : public ISystem {
public:
  const std::string_view name() override {
    return "core::system::Window";
  }

  Window(entt::registry& registry): ISystem(registry) {
    shutdown_service = core::service::get_service<core::service::Shutdown>();
  };
  void update() override {
    shutdown_service->force_close();
  }
private:
  std::shared_ptr<core::service::Shutdown> shutdown_service;
};

} /* core::systems */
