#pragma once

#include <core/ISystem.hpp>
#include <core/service/Window.hpp>
#include <core/service/PerformanceController.hpp>


namespace ss13::system {

class Init final : public core::ISystem {
public:
  const std::string_view name() const noexcept override;
  void init() override;
  void update() override;
private:
  std::shared_ptr<core::service::Window> window_service;
  std::shared_ptr<core::service::PerformanceController> performance_service;
  sf::Font font;
  sf::Text text;
  std::string string;
};


} /* ss13::system */
