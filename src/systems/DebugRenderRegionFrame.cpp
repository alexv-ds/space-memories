#include "system_update_priority.hpp"
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
  void setup(Settings& settings) const override {
    settings.priority = update_priority::DebugRenderRegionFrame;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::DebugRenderRegionFrame>();
    view.each([&registry, this](auto entity, auto& camera, auto& debug_frame) {
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      sf::FloatRect r_region = camera_service->get_render_region(entity, registry);
      shape.setPosition({r_region.left, r_region.top});
      shape.setSize({r_region.width,r_region.height});
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
