#pragma once

#include "service_helpers.hpp"
#include <core/service/Shutdown.hpp>
#include <SFML/Graphics.hpp>

namespace core::service {

class Window {
public:
  static constexpr const std::string_view name = "core::service::Window";

  Window() {
    shutdown_service = get_service<Shutdown>();
  }
  ~Window() {
    if (window.isOpen()) {
      window.close();
    }
  }

  void update() {
    if (!window.isOpen()) {
      shutdown_service->graceful_close();
      return;
    }
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        shutdown_service->graceful_close();
        return;
      }
      window.display();
    }
  }


private:
  std::shared_ptr<Shutdown> shutdown_service;
  sf::RenderWindow window = {sf::VideoMode(800, 600), "Space Memories"};

};



} /* core::service */
