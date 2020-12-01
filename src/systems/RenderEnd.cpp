#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Camera.hpp>
#include <components/RenderWindow.hpp>

namespace {

class RenderEnd final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::shared_ptr<service::Camera> camera;
public:
  RenderEnd(std::shared_ptr<service::SFMLRenderWindow>, std::shared_ptr<service::Camera>);
  void update(entt::registry& registry) override {
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (window) {
        window->display();
      }
    }
    camera->render_end();
  }
};

RenderEnd::RenderEnd(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window,
                     std::shared_ptr<service::Camera> camera):
  sfml_render_window(std::move(sfml_render_window)),
  camera(std::move(camera))
{}

CORE_DEFINE_SYSTEM("system::RenderEnd", [](core::ServiceLocator& locator){
  return std::make_unique<RenderEnd>(
    std::move(locator.get<service::SFMLRenderWindow>()),
    std::move(locator.get<service::Camera>())
  );
});

}