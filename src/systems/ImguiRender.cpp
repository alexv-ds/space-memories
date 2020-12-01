#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <imgui-SFML.h>

namespace {

class ImguiRender final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
public:
  ImguiRender(std::shared_ptr<service::SFMLRenderWindow>);
  void update(entt::registry& registry) override {
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (!window) {
        continue;
      }
      ImGui::SFML::SetCurrentWindow(*window);      
      ImGui::SFML::Render(*window); 
    }
  }
};

ImguiRender::ImguiRender(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window):
  sfml_render_window(std::move(sfml_render_window))
{}

CORE_DEFINE_SYSTEM("system::ImguiRender", [](core::ServiceLocator& locator){
  return std::make_unique<ImguiRender>(std::move(locator.get<service::SFMLRenderWindow>()));
});

}