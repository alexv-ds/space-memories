#pragma once

#include <core/ISystem.hpp>
#include <core/service/Window.hpp>
#include <core/service/PerformanceController.hpp>
#include <core/service/TextureLoader.hpp>


namespace ss13::system {

class Init final : public core::ISystem {
public:
  const std::string_view name() const noexcept override;
  void init() override;
  void update() override;
private:
  std::shared_ptr<core::service::Window> window_service;
  std::shared_ptr<core::service::PerformanceController> performance_service;
  std::shared_ptr<core::service::TextureLoader> texture_loader;
  sf::Font font;
  sf::Text text;
  char buffer[64];
};


} /* ss13::system */
