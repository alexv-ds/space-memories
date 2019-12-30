#include "WindowClear.hpp"

namespace core::system {

const std::string_view WindowClear::name() const noexcept {
  return "core::system::WindowClear";
}
WindowClear::WindowClear() {
  this->window_service = service::get_service<service::Window>();
}
void WindowClear::update() {
  sf::RenderTarget* const render = this->window_service->get_render_target();
  if (render) {
    render->clear();
  }
}


} /* core::system */
