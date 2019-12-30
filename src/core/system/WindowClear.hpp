#pragma once

#include <core/ISystem.hpp>
#include <core/service/Window.hpp>

namespace core::system {

class WindowClear final : public ISystem {
public:
  const std::string_view name() const noexcept override;
  WindowClear();
  void update() override;
private:
  std::shared_ptr<service::Window> window_service;
};

} /* core::system */
