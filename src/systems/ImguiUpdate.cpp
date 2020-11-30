#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Input.hpp>
#include <SFML/System/Clock.hpp>
#include <imgui-SFML.h>

namespace {

class ImguiUpdate final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  sf::Clock delta_clock;
public:
  ImguiUpdate(std::shared_ptr<service::SFMLRenderWindow>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::ImguiUpdate;
  }
  void update(entt::registry& registry) override {
    sf::Time delta = delta_clock.restart();
    for (entt::entity entity : registry.view<component::RenderWindow>()) {
      sf::RenderWindow* window = sfml_render_window->get_window(entity);
      if (!window) {
        continue;
      }
      ImGui::SFML::SetCurrentWindow(*window);
      const component::ListenWindowEvents* p_input = registry.try_get<component::ListenWindowEvents>(entity);
      for (const sf::Event& event : p_input->events) {
        ImGui::SFML::ProcessEvent(event);
      }
      ImGui::SFML::Update(*window, delta);
    }
  }
};

ImguiUpdate::ImguiUpdate(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window):
  sfml_render_window(std::move(sfml_render_window))
{}

CORE_DEFINE_SYSTEM("system::ImguiUpdate", [](core::ServiceLocator& locator){
  return std::make_unique<ImguiUpdate>(std::move(locator.get<service::SFMLRenderWindow>()));
});

}