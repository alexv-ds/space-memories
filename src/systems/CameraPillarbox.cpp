#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <limits>
#include <cmath>

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
  {
    shape.setFillColor({0,0,0,0});
    shape.setOutlineColor(sf::Color::Black);
  }

  void setup(Settings& settings) const override {
    settings.priority = update_priority::CameraPillarbox;
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraPillarbox>();
    view.each([&registry, this](entt::entity entity, const auto& cam){
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      sf::FloatRect region = camera_service->get_render_region(entity, registry);

      shape.setPosition({region.left, region.top});
      shape.setSize({region.width, region.height});

      sf::Vector2u r_target_size = r_target->getSize();
      sf::Vector2f region_size = region.getSize();
      float horizontal_space = static_cast<float>(r_target_size.x) - region_size.x;
      float vertical_space = static_cast<float>(r_target_size.y) - region_size.y;
      shape.setOutlineThickness(
        (horizontal_space > vertical_space ? horizontal_space : vertical_space ) * 1.1f
      );
      r_target->draw(shape);
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
