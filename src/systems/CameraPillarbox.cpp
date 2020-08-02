#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <limits>
#include <cmath>
#include <iostream>
namespace {

class CameraPillarbox final : public core::System {
  std::shared_ptr<service::SFMLRenderWindow> rwindow_service;
  std::shared_ptr<service::Camera> camera_service;
  sf::RectangleShape shape;
public:
  CameraPillarbox(std::shared_ptr<service::SFMLRenderWindow> rwindow,
                  std::shared_ptr<service::Camera> camera):
      //init
      rwindow_service(std::move(rwindow)),
      camera_service(std::move(camera))
  {}

  void setup(Settings& settings) const override {
    settings.priority = update_priority::CameraPillarbox;
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera,
                              component::CameraPillarbox>();
    view.each([&registry, this](entt::entity entity, auto& cam){
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      sf::View old_view = r_target->getView();
      sf::View view = camera_service->calculate_camera_view(cam, old_view);
      r_target->setView(view);
      sf::Vector2f view_abs_size = view.getSize();
      view_abs_size.x = std::abs(view_abs_size.x);
      view_abs_size.y = std::abs(view_abs_size.y);
      shape.setFillColor(sf::Color::Black);
      if (view_abs_size.x - cam.size_x > std::numeric_limits<float>::epsilon()) {
        float half_delta = (view_abs_size.x - cam.size_x) * 0.55; //Чуть больше половины из-за просветов
        shape.setSize({half_delta, cam.size_y});
        shape.setPosition({cam.size_x, 0.0f});
      } else {
        float half_delta = (view_abs_size.y - cam.size_y) * 0.55; //Чуть больше половины из-за просветов
        shape.setSize({cam.size_x, half_delta});
        shape.setPosition({0.0f, cam.size_y});
      }
      r_target->draw(shape);
      r_target->setView(old_view);
    });
  }
};

CORE_DEFINE_SYSTEM("system::CameraPillarbox", [](core::ServiceLocator& locator){
  return std::make_unique<CameraPillarbox>(
    locator.get<service::SFMLRenderWindow>(),
    locator.get<service::Camera>()
  );
});

}
