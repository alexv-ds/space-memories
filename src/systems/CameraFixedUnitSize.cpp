#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Camera.hpp>

namespace {

class CameraFixedUnitSize : public core::System {
  std::shared_ptr<service::Camera> camera_service;
public:
  CameraFixedUnitSize(std::shared_ptr<service::Camera> camera_service):
    camera_service(std::move(camera_service))
  {}
  void setup(Settings& settings) const override {
    settings.priority = update_priority::CameraFixedUnitSize;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraFixedUnitSize>();

    view.each([&registry, this](auto entity, auto& camera, auto& unit_size) {
      sf::Vector2f render_size = camera_service->get_render_region(entity, registry).getSize();
      camera.size_x = render_size.x / unit_size.x;
      camera.size_y = render_size.y / unit_size.y;
    });
  }
};

CORE_DEFINE_SYSTEM("system::CameraFixedUnitSize", [](core::ServiceLocator& locator){
  return std::make_unique<CameraFixedUnitSize>(
    locator.get<service::Camera>()
  );
});


}
