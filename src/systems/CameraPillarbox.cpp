#include <core/define_system.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <limits>
#include <cmath>

namespace {

class CameraPillarbox final : public core::System {
  std::shared_ptr<service::Camera> camera_service;
  sf::RectangleShape shape;
public:
  CameraPillarbox(std::shared_ptr<service::Camera> camera):
      //init
      camera_service(std::move(camera))
  {
    shape.setFillColor({0,0,0,0});
    shape.setOutlineColor(sf::Color::Black);
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraRenderRegion, component::CameraPillarbox>();
    view.each([&registry, this](entt::entity entity, const auto& cam, const auto& render_region){
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }

      shape.setPosition({render_region.rect.left, render_region.rect.top});
      shape.setSize({render_region.rect.width, render_region.rect.height});

      sf::Vector2u r_target_size = r_target->getSize();
      sf::Vector2f region_size = render_region.rect.getSize();
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
    locator.get<service::Camera>()
  );
});

}
