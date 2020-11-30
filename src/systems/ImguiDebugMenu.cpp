#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Input.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <components/Imgui.hpp>

namespace {

class ImguiDebugMenu final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
public:
  ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::ImguiDebugMenu;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ImguiDebugMenu, component::RenderWindow>();
    view.each([this](auto entity) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (!window) {
        return;
      }
      ImGui::SFML::SetCurrentWindow(*window);
      ImGui::Begin("Hello, world!");
      ImGui::End();
    });
  }
};

ImguiDebugMenu::ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window):
  sfml_render_window(std::move(sfml_render_window))
{}

CORE_DEFINE_SYSTEM("system::ImguiDebugMenu", [](core::ServiceLocator& locator){
  return std::make_unique<ImguiDebugMenu>(std::move(locator.get<service::SFMLRenderWindow>()));
});

}