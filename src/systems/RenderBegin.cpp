#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Camera.hpp>
#include <components/RenderWindow.hpp>

namespace {

class RenderBegin final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::shared_ptr<service::Camera> camera;
public:
  RenderBegin(std::shared_ptr<service::SFMLRenderWindow>, std::shared_ptr<service::Camera>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::RenderBegin;
  }
  void update(entt::registry& registry) override {
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (window) {
        sf::Vector2u window_size = window->getSize();
        window->setView(
          sf::View({0.0f, 0.0f, static_cast<float>(window_size.x), static_cast<float>(window_size.y)})
        );
        window->clear(sf::Color::Black);
      }
    }
    camera->render_begin();
  }
};

RenderBegin::RenderBegin(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window,
                         std::shared_ptr<service::Camera> camera):
  sfml_render_window(std::move(sfml_render_window)),
  camera(std::move(camera))
{}

CORE_DEFINE_SYSTEM("system::RenderBegin", [](core::ServiceLocator& locator){
  return std::make_unique<RenderBegin>(
    std::move(locator.get<service::SFMLRenderWindow>()),
    std::move(locator.get<service::Camera>())
  );
});

}