#include <core/define_system.hpp>
#include <core/SystemManager.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Input.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <components/Imgui.hpp>
#include <map>

namespace {

class ImguiMenuToggler {
public:
  const char* name;
  
  ImguiMenuToggler(const char* name): name(name) {}
  virtual void toggle(entt::registry&, entt::entity) = 0;
  virtual ~ImguiMenuToggler() = default;
};

template <class T>
class ImguiMenuTogglerImpl final : public ImguiMenuToggler {
  using ShowMenuFunc = std::function<void(entt::registry&, entt::entity)>;
public:
  ImguiMenuTogglerImpl(const char* name): ImguiMenuToggler(name) {};
  void toggle(entt::registry& registry, entt::entity entity) override {
    if (registry.has<T>(entity)) {
      registry.remove<T>(entity);
    } else {
      registry.emplace<T>(entity);
    }
  }
};

template <class T>
std::unique_ptr<ImguiMenuToggler> create_toggler(const char* name) {
  return std::make_unique<ImguiMenuTogglerImpl<T>>(name);
}

class ImguiDebugMenu final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::shared_ptr<core::SystemManager> system_manager;
  entt::observer keyboad_update_observer;
  entt::registry* p_registry = nullptr;
  std::map<sf::Keyboard::Key, std::unique_ptr<ImguiMenuToggler>> menu_activators;
  
public:
  ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow>,
                 std::shared_ptr<core::SystemManager>);
  ~ImguiDebugMenu() {
    if (p_registry) {
      keyboad_update_observer.disconnect();
    }
  }
  void init(entt::registry& registry) override {
    p_registry = &registry;
    keyboad_update_observer.connect(
      *p_registry,
      entt::collector.update<component::ListenKeyboard>().where<component::UseDebugMenus>()
    );
  }
  void update(entt::registry& registry) override {
    process_menu_activators(registry);
    debug_system_menu(registry);
    //ImGui::ShowDemoWindow();
  }
  
  void process_menu_activators(entt::registry& registry);
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
{
  menu_activators[sf::Keyboard::F2] = create_toggler<component::DebugSystemMenu>("System Menu");
}

void ImguiDebugMenu::process_menu_activators(entt::registry& registry) {
  keyboad_update_observer.each([this, &registry](auto entity) {
    const auto& keyboard = registry.get<component::ListenKeyboard>(entity);
    for (const auto& [key, state] : keyboard.keys) {
      auto menu_activators_it = menu_activators.find(key);
      if (menu_activators_it == menu_activators.end()) {
        continue;
      }
      if (state.down) {
        menu_activators_it->second->toggle(registry, entity);
      }
    }    
  });  
}

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