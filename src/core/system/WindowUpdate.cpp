#include "WindowUpdate.hpp"

namespace core::system {
//
const std::string_view WindowUpdate::name() const noexcept {
  return "core::system::WindowUpdate";
}
WindowUpdate::WindowUpdate() {
  this->window_service = service::get_service<service::Window>();
}
void WindowUpdate::update() {
  this->window_service->update();
}

} /* core::system */
