#pragma once

#include <core/ISystem.hpp>
#include <core/service/Window.hpp>
#include <core/service/PerformanceController.hpp>

namespace ss13::system {

class TickUsageCounter final : public core::ISystem {
public:
  const std::string_view name() const noexcept override;
  void init() override;
  void update() override;
private:
  std::shared_ptr<core::service::Window> window;
  std::shared_ptr<core::service::PerformanceController> performance;
  sf::Font font;
  sf::Text text;
  char buffer[64];
  float accumulated_tick_usage = 0;
  size_t counter = 0;
};


} /* ss13::system */
