#include <core/define_system.hpp>
#include <core/SystemManager.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Time.hpp>
#include <components/Input.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <components/Imgui.hpp>
#include <map>
#include <algorithm>
#include <cmath>

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
  std::shared_ptr<service::Time> time_service;
  entt::observer keyboad_update_observer;
  entt::registry* p_registry = nullptr;
  std::map<sf::Keyboard::Key, std::unique_ptr<ImguiMenuActivator>> menu_activators;
  std::vector<entt::entity> to_remove_entities_buffer; //используется в функциях рисования меню
  
public:
  ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow>,
                 std::shared_ptr<core::SystemManager>,
                 std::shared_ptr<service::Time>);
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
    std::move(locator.get<core::SystemManager>()),
    std::move(locator.get<service::Time>())
  );
});

ImguiDebugMenu::ImguiDebugMenu(std::shared_ptr<service::SFMLRenderWindow> sfml_render_window,
                               std::shared_ptr<core::SystemManager> system_manager,
                               std::shared_ptr<service::Time> time_service):
  sfml_render_window(std::move(sfml_render_window)),
  system_manager(std::move(system_manager)),
  time_service(std::move(time_service))
{
  menu_activators[sf::Keyboard::F1] = create_activator<component::DebugMenusList>("Debug Menus List");
  menu_activators[sf::Keyboard::F2] = create_activator<component::DebugSystemMenu>("Systems");
  menu_activators[sf::Keyboard::F3] = create_activator<component::DebugSystemPerformance>("Systems Performance");
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

void collect_time(component::DebugSystemPerformance::TimeData& time_data, double time) {
  time_data.total += time;
  if (time_data.max < time) {
    time_data.max = time;
  }
  ++time_data.collect_times;
  time_data.current = time;
}

void update_visualise(float period,
                      const std::string& name,
                      component::DebugSystemPerformance::TimeData& time_data, 
                      component::DebugSystemPerformance::ForVisualiseData& visualise_data)
{
  visualise_data.max = time_data.max;
  visualise_data.avg = time_data.total / time_data.collect_times;
  //Округляем до 5 знаков после запятой
  //При дальнейшем переводе до милисекунд огругление идет до 2х после запятой
  visualise_data.avg = std::floor(visualise_data.avg * 100000.0) / 100000.0;
  visualise_data.name = name;
  time_data = component::DebugSystemPerformance::TimeData(); //Зануляем все значения
}

void imgui_draw_performance_data(double max, double avg, double current, const std::string& name) {
  //Если нужно поменять количество знакоп после запятой, то не забывать править округление
  //в update_visualise();
  ImGui::Text("%5.2lf", avg * 1000); ImGui::NextColumn();
  ImGui::Text("%5.2lf", max * 1000); ImGui::NextColumn();
  ImGui::Text("%5.2lf", current * 1000); ImGui::NextColumn();
  ImGui::Text("%s", name.c_str()); ImGui::NextColumn();
}

//Тут черт ногу сломит нахуй, не дай бог это дебажить
void ImguiDebugMenu::debug_system_performance_menu(entt::registry& registry) {
  to_remove_entities_buffer.clear();
  auto view = registry.view<component::DebugSystemPerformance, component::RenderWindow>();
  //performance не по константной ссылке, потому что нам не нужны триггеры обновления, и менять мы будем напрямую
  //То, что этот компонент меняется без учета триггеров дожно быть указано в его хедере
  view.each([this, &registry](auto entity, auto& performance) {
    sf::RenderWindow* window = sfml_render_window->get_window(entity);
    if (!window) {
      return;
    }
    ImGui::SFML::SetCurrentWindow(*window);
    
    //Собираем время и обновляем TimeData структуры
    collect_time(performance.all_systems_data, system_manager->get_systems_update_time());
    system_manager->each_system([this, &performance](const core::SystemManager::SystemStatus& status) {
      collect_time(performance.systems[status.name], status.execution_time);
    });
    performance.current_real_time = time_service->get_real_time();
    float period = performance.current_real_time - performance.start_real_time;
    
    //Обновляем ForVisualiseData на основе TimeData
    if (period >= 1.0f) {
      performance.start_real_time = performance.current_real_time;
      update_visualise(period, "", performance.all_systems_data, performance.all_systems_visualise);
      performance.for_visualise.clear();
      for (auto& [name, time_data] : performance.systems) {
        performance.for_visualise.emplace_back();
        update_visualise(period, name, time_data, performance.for_visualise.back());
      }
      std::sort(performance.for_visualise.begin(), performance.for_visualise.end(), [](const auto& a, const auto& b) {
        return a.avg > b.avg;
      });      
    }
    
    //Рисуем меню
    bool show_window = true;
    if (ImGui::Begin("Systems Performance", &show_window, ImGuiWindowFlags_NoCollapse)) {
      ImGui::Columns(4);
      ImGui::Text("Average, ms"); ImGui::NextColumn();
      ImGui::Text("Maximum, ms"); ImGui::NextColumn();
      ImGui::Text("Current, ms"); ImGui::NextColumn();
      ImGui::Text("System"); ImGui::NextColumn();
      ImGui::Separator();
      imgui_draw_performance_data(
        performance.all_systems_visualise.max,
        performance.all_systems_visualise.avg,
        performance.all_systems_data.current,
        "Total"
      );
      ImGui::Separator();
      for (const auto& visualise_data : performance.for_visualise) {
        imgui_draw_performance_data(
          visualise_data.max,
          visualise_data.avg,
          performance.systems[visualise_data.name].current,
          visualise_data.name
        );
      }
    }
    if (!show_window) {
      to_remove_entities_buffer.push_back(entity);
    }
    ImGui::End();
  });
  registry.remove<component::DebugSystemPerformance>(to_remove_entities_buffer.begin(), to_remove_entities_buffer.end());   
}

}