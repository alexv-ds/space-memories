#include <core/define_system.hpp>
#include <core/SystemManager.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Input.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <components/Imgui.hpp>
#include <cmath>

namespace {

class ImguiDebugMenu final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::shared_ptr<core::SystemManager> system_manager;
public:
  ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow>,
                 std::shared_ptr<core::SystemManager>);
  void update(entt::registry& registry) override {
    debug_system_menu(registry);
    //ImGui::ShowDemoWindow();
  }
  
  void debug_system_menu(entt::registry& registry);
};

CORE_DEFINE_SYSTEM("system::ImguiDebugMenu", [](core::ServiceLocator& locator){
  return std::make_unique<ImguiDebugMenu>(
    std::move(locator.get<service::SFMLRenderWindow>()),
    std::move(locator.get<core::SystemManager>())
  );
});

ImguiDebugMenu::ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window,
                               std::shared_ptr<core::SystemManager> system_manager):
  sfml_render_window(std::move(sfml_render_window)),
  system_manager(std::move(system_manager))
{}



void ImguiDebugMenu::debug_system_menu(entt::registry& registry) {
  std::vector<entt::entity> to_remove_entities;
  
  auto view = registry.view<component::DebugSystemMenu, component::RenderWindow>();
  view.each([this, &to_remove_entities](auto entity) {
    sf::RenderWindow* window = sfml_render_window->get_window(entity);
    if (!window) {
      return;
    }
    ImGui::SFML::SetCurrentWindow(*window);
    bool show_window = true;
    if (ImGui::Begin("Systems", &show_window, ImGuiWindowFlags_NoCollapse)) {
      ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f,0.5f));
      system_manager->each_system([this](const core::SystemManager::SystemStatus& status) {
        if (ImGui::Button(status.name.c_str(), {300,0})) {
          system_manager->toggle_system(status.name);
        }
        ImGui::SameLine();
        if (status.active) {
          ImGui::TextColored({0,255,0,255}, "ON");
        } else {
          ImGui::TextColored({255,0,0,255}, "OFF");
        }
      });
      ImGui::PopStyleVar();
      
    }
    if (!show_window) {
      to_remove_entities.push_back(entity);
    }
    ImGui::End();
  });
  registry.remove<component::DebugSystemMenu>(to_remove_entities.begin(), to_remove_entities.end());
}

}