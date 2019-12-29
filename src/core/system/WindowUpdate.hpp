#pragma once
#include <core/service/Window.hpp>

namespace core::system {

class WindowUpdate final : public ISystem {
public:
  const std::string_view name() const noexcept override {
    return "core::system::WindowUpdate";
  }
  WindowUpdate() {
    this->window_service = service::get_service<service::Window>();
  }
  void update() override {
    window_service->update();
  }
private:
  std::shared_ptr<service::Window> window_service;
};

} /* core::system */
