#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/RenderWindow.hpp>
#include "system_update_priority.hpp"

namespace {

class SFMLRenderWindow final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> render_window_service;

  //Временный костыль
  entt::registry* registry = nullptr;

public:
  SFMLRenderWindow(std::shared_ptr<service::SFMLRenderWindow>);
  void setup(Settings& settings) const override {
    settings.priority = update_priority::SFMLRenderWindow;
  }

  void init(entt::registry& registry) override {
    this->registry = &registry;
    registry.view<component::RenderWindow>().each([this](auto entity, auto& window) {
      render_window_service->update_window(entity, window);
    });
    registry.on_construct<component::RenderWindow>().connect<&SFMLRenderWindow::on_construct_cb>(this);
    registry.on_destroy<component::RenderWindow>().connect<&SFMLRenderWindow::on_destroy_cb>(this);
  }
  void update(entt::registry& registry) override {
    registry.view<component::RenderWindow>().each([this](auto entity, auto& window) {
      render_window_service->update_window(entity, window);
    });
  }

  ~SFMLRenderWindow() {
    if (registry) {
      registry->on_construct<component::RenderWindow>().disconnect<&SFMLRenderWindow::on_construct_cb>(this);
      registry->on_destroy<component::RenderWindow>().disconnect<&SFMLRenderWindow::on_destroy_cb>(this);
    }
  }

  void on_construct_cb(entt::registry& registry, entt::entity entity) {
    render_window_service->update_window(entity, registry.get<component::RenderWindow>(entity));
  }
  void on_destroy_cb(entt::registry& registry, entt::entity entity) {
    render_window_service->destroy_window(entity);
  }
};

SFMLRenderWindow::SFMLRenderWindow(std::shared_ptr<service::SFMLRenderWindow> render_window_service):
  render_window_service(std::move(render_window_service))
{}

CORE_DEFINE_SYSTEM("system::SFMLRenderWindow", [](core::ServiceLocator& locator) {
  return std::make_unique<SFMLRenderWindow>(std::move(locator.get<service::SFMLRenderWindow>()));
});

}

