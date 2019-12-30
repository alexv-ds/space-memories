#pragma once

#include "service_helpers.hpp"
#include <core/service/Shutdown.hpp>
#include <SFML/Graphics.hpp>

namespace core::service {

class Window {
public:
  static constexpr const std::string_view name = "core::service::Window";

  Window();
  ~Window();
  void update();

  //может вернуть nullptr
  inline sf::RenderTarget* get_render_target() noexcept;

private:
  std::shared_ptr<Shutdown> shutdown_service;
  sf::RenderWindow window;

  void handle_resize_event(sf::Event&) noexcept;

};


//inline methods
inline sf::RenderTarget* Window::get_render_target() noexcept {
  return this->window.isOpen() ? &this->window : nullptr;
}


} /* core::service */
