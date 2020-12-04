#include <core/define_system.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace {

class DebugRenderRegionFrame : public core::System {
  std::shared_ptr<service::Camera> camera_service;
  sf::RectangleShape shape;
public:
  DebugRenderRegionFrame(std::shared_ptr<service::Camera> camera_service):
    camera_service(std::move(camera_service))
  {
    shape.setFillColor({0,0,0,0});
    shape.setOutlineThickness(2.0f);
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::DebugRenderRegionFrame, component::CameraRenderRegion>();
    view.each([&registry, this](auto entity, const auto& camera, const auto& debug_frame, const auto& render_region) {
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      shape.setPosition({render_region.rect.left, render_region.rect.top});
      shape.setSize({render_region.rect.width,render_region.rect.height});
      shape.setOutlineColor(debug_frame.color);
      r_target->draw(shape);
    });
  }
};




CORE_DEFINE_SYSTEM("system::DebugRenderRegionFrame", [](core::ServiceLocator& locator){
  return std::make_unique<DebugRenderRegionFrame>(
    locator.get<service::Camera>()
  );
});


}
