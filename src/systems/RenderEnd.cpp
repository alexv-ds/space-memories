#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/RenderWindow.hpp>

namespace {

class RenderEnd final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
public:
  RenderEnd(std::shared_ptr<service::SFMLRenderWindow>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::RenderEnd;
  }
  void update(entt::registry& registry) override {
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (window) {
        window->display();
      }
    }
  }
};

RenderEnd::RenderEnd(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window):
  sfml_render_window(move(sfml_render_window))
{}

CORE_DEFINE_SYSTEM("system::RenderEnd", [](core::ServiceLocator& locator){
  return std::make_unique<RenderEnd>(std::move(locator.get<service::SFMLRenderWindow>()));
});

}