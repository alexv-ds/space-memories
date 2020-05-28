#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Input.hpp>
#include <components/RenderWindow.hpp>
#include <components/Input.hpp>
#include <SFML/Window.hpp>

namespace {

class WindowEventsHandler : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> render_window_service;
  std::shared_ptr<service::Input> input_service;
  std::vector<sf::Event> close; //sf::Event::Closed
  std::vector<sf::Event> size; //sf::Event::Resized 
  std::vector<sf::Event> focus; //sf::Event::LostFocus, sf::Event::GainedFocus
  std::vector<sf::Event> text; //sf::Event::TextEntered
  std::vector<sf::Event> key; //sf::Event::KeyPressed, sf::Event::KeyReleased
  std::vector<sf::Event> mouse_wheel; //sf::Event::MouseWheelScrolled
  std::vector<sf::Event> mouse_button; //sf::Event::MouseButtonPressed, sf::Event::MouseButtonReleased
  std::vector<sf::Event> mouse_move; //sf::Event::MouseMoved
  std::vector<sf::Event> mouse_enter; //sf::Event::MouseEntered, sf::Event::MouseLeft

public:
  WindowEventsHandler(std::shared_ptr<service::SFMLRenderWindow>,
                      std::shared_ptr<service::Input>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::WindowEventHandler;
  }
  void update(entt::registry& registry) override {
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = render_window_service->get_window(entity);
      if (!window) {
        continue;
      }
      poll_events(window);
      process_close_vector(registry, entity);
      process_key_vector(registry, entity);
      clear();
    }
  }
private:
  void poll_events(sf::RenderWindow* window) {
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed: {
          close.push_back(event);
          break;
        }
        case sf::Event::Resized: {
          size.push_back(event);
          break;
        }
        case sf::Event::LostFocus:
        case sf::Event::GainedFocus: {
          focus.push_back(event);
          break;
        }
        case sf::Event::TextEntered: {
          text.push_back(event);
          break;
        }
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased: {
          key.push_back(event);
          break;
        }
        case sf::Event::MouseWheelScrolled: {
          mouse_wheel.push_back(event);
          break;
        }
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased: {
          mouse_button.push_back(event);
          break;
        }
        case sf::Event::MouseMoved: {
          mouse_move.push_back(event);
          break;
        }
        case sf::Event::MouseEntered:
        case sf::Event::MouseLeft: {
          mouse_enter.push_back(event);
        }
        default: {}
      } //switch
    } //while
  };

  void clear() {
    close.clear();
    size.clear();
    focus.clear();
    text.clear();
    key.clear();
    mouse_wheel.clear();
    mouse_button.clear();
    mouse_move.clear();
    mouse_enter.clear();
  }

  void process_close_vector(entt::registry& registry, entt::entity entity) {
    if (close.size()) {
      registry.remove_if_exists<component::RenderWindow>(entity);
    }
  }

  void process_key_vector(entt::registry& registry, entt::entity entity) {
    if (key.size() && registry.has<component::ListenKeyboard>(entity)) {
      input_service->update_keys(entity, key);
    }
  }

};



WindowEventsHandler::WindowEventsHandler(std::shared_ptr<service::SFMLRenderWindow> render_window_service,
                                         std::shared_ptr<service::Input> input_service):
  input_service(std::move(input_service)),
  render_window_service(std::move(render_window_service))
{}

CORE_DEFINE_SYSTEM("system::WindowEventsHandler", [](core::ServiceLocator& locator) {
  return std::make_unique<WindowEventsHandler>(
    std::move(locator.get<service::SFMLRenderWindow>()),
    std::move(locator.get<service::Input>())
    );
});



}