#include <core/define_system.hpp>
#include <core/SystemManager.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Input.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <components/Imgui.hpp>
#include <map>

namespace {

class ImguiMenuActivator {
public:
  const char* name;
  
  ImguiMenuActivator(const char* name): name(name) {}
  virtual void toggle(entt::registry&, entt::entity) = 0;
  virtual ~ImguiMenuActivator() = default;
};

template <class T>
class ImguiMenuActivatorImpl final : public ImguiMenuActivator {
  using ShowMenuFunc = std::function<void(entt::registry&, entt::entity)>;
public:
  ImguiMenuActivatorImpl(const char* name): ImguiMenuActivator(name) {};
  void toggle(entt::registry& registry, entt::entity entity) override {
    if (registry.has<T>(entity)) {
      registry.remove<T>(entity);
    } else {
      registry.emplace<T>(entity);
    }
  }
};

template <class T>
std::unique_ptr<ImguiMenuActivator> create_activator(const char* name) {
  return std::make_unique<ImguiMenuActivatorImpl<T>>(name);
}

class ImguiDebugMenu final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::shared_ptr<core::SystemManager> system_manager;
  entt::observer keyboad_update_observer;
  entt::registry* p_registry = nullptr;
  std::map<sf::Keyboard::Key, std::unique_ptr<ImguiMenuActivator>> menu_activators;
  std::vector<entt::entity> to_remove_entities_buffer; //используется в функциях рисования меню
  
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
    debug_menus_list(registry);
    debug_system_performance_menu(registry);
    debug_imgui_demo(registry);
    
  }
  
  void process_menu_activators(entt::registry& registry);
  void debug_system_menu(entt::registry& registry);
  void debug_menus_list(entt::registry& registry);
  void debug_imgui_demo(entt::registry& registry);
  void debug_system_performance_menu(entt::registry& registry);
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
  menu_activators[sf::Keyboard::F1] = create_activator<component::DebugMenusList>("Debug Menus List");
  menu_activators[sf::Keyboard::F2] = create_activator<component::DebugSystemMenu>("System");
  menu_activators[sf::Keyboard::F3] = create_activator<component::DebugSystemPerformance>("System Performance");
  menu_activators[sf::Keyboard::F4] = create_activator<component::DebugImguiDemoMenu>("Imgui Demo");
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
  to_remove_entities_buffer.clear();
  
  auto view = registry.view<component::DebugSystemMenu, component::RenderWindow>();
  view.each([this](auto entity) {
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
      to_remove_entities_buffer.push_back(entity);
    }
    ImGui::End();
  });
  registry.remove<component::DebugSystemMenu>(to_remove_entities_buffer.begin(), to_remove_entities_buffer.end());
}

void ImguiDebugMenu::debug_menus_list(entt::registry& registry) {
  to_remove_entities_buffer.clear();
  auto view = registry.view<component::DebugMenusList, component::RenderWindow>();
  view.each([this, &registry](auto entity) {
    sf::RenderWindow* window = sfml_render_window->get_window(entity);
    if (!window) {
      return;
    }
    ImGui::SFML::SetCurrentWindow(*window);
    bool show_window = true;
    if (ImGui::Begin("Debug Menus List", &show_window, ImGuiWindowFlags_NoCollapse)) {
      for (auto& [key, p_menu_activator] : menu_activators) {
        if (ImGui::Button(p_menu_activator->name, {200,0})) {
          p_menu_activator->toggle(registry, entity);
        }
      }
    }
    if (!show_window) {
      to_remove_entities_buffer.push_back(entity);
    }
    ImGui::End();
  });
  registry.remove<component::DebugMenusList>(to_remove_entities_buffer.begin(), to_remove_entities_buffer.end()); 
}

void ImguiDebugMenu::debug_imgui_demo(entt::registry& registry) {
  to_remove_entities_buffer.clear();
  auto view = registry.view<component::DebugImguiDemoMenu, component::RenderWindow>();
  view.each([this, &registry](auto entity) {
    sf::RenderWindow* window = sfml_render_window->get_window(entity);
    if (!window) {
      return;
    }
    ImGui::SFML::SetCurrentWindow(*window);
    bool show_window = true;
    ImGui::ShowDemoWindow(&show_window);
    if (!show_window) {
      to_remove_entities_buffer.push_back(entity);
    }
  });
  registry.remove<component::DebugImguiDemoMenu>(to_remove_entities_buffer.begin(), to_remove_entities_buffer.end());   
}

void ImguiDebugMenu::debug_system_performance_menu(entt::registry& registry) {
  to_remove_entities_buffer.clear();
  auto view = registry.view<component::DebugSystemPerformance, component::RenderWindow>();
  view.each([this, &registry](auto entity) {
    sf::RenderWindow* window = sfml_render_window->get_window(entity);
    if (!window) {
      return;
    }
    ImGui::SFML::SetCurrentWindow(*window);
    bool show_window = true;
    if (ImGui::Begin("System Performance", &show_window, ImGuiWindowFlags_NoCollapse)) {
    
    }
    if (!show_window) {
      to_remove_entities_buffer.push_back(entity);
    }
    ImGui::End();
  });
  registry.remove<component::DebugSystemPerformance>(to_remove_entities_buffer.begin(), to_remove_entities_buffer.end());   
}

}