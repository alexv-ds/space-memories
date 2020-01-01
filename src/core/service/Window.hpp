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
  inline sf::RenderWindow* get_render_window() noexcept;

private:
  std::shared_ptr<Shutdown> shutdown_service;
  sf::RenderWindow window;
  sf::FloatRect visible_area;

  void handle_resize_event(sf::Event&) noexcept;

};


//inline methods
inline sf::RenderWindow* Window::get_render_window() noexcept {
  return this->window.isOpen() ? &this->window : nullptr;
}


} /* core::service */
