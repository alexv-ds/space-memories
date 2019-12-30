#include "Window.hpp"

#include <algorithm>

namespace core::service {

Window::Window() {
  this->shutdown_service = get_service<Shutdown>();

  sf::VideoMode vmode = sf::VideoMode::getDesktopMode();
  vmode.width = static_cast<float>(vmode.width) / 1.3f;
  vmode.height = static_cast<float>(vmode.height) / 1.3f;

  this->window.create(vmode,"Space Memories");
}

Window::~Window() {
  if (this->window.isOpen()) {
    this->window.close();
  }
}

void Window::update() {
  if (!this->window.isOpen()) {
    this->shutdown_service->graceful_close();
    return;
  }
  sf::Event event;
  while (this->window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      this->window.close();
      this->shutdown_service->graceful_close();
      return;
    } else if (event.type == sf::Event::Resized) {
      this->handle_resize_event(event);
    }
    this->window.display();
  }
}

void Window::handle_resize_event(sf::Event& event) noexcept {
  sf::VideoMode vmode = sf::VideoMode::getDesktopMode();
  unsigned int min_width = static_cast<float>(vmode.width) / 1.3f;
  unsigned int min_height = static_cast<float>(vmode.height) / 1.3f;

  if (event.size.width < min_width || event.size.height < min_height) {
    this->window.setSize({
      std::max(event.size.width, min_width),
      std::max(event.size.height, min_height)
    });
  }
}

} /* core::service */
