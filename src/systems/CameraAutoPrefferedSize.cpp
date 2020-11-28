#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>

namespace {

class CameraAutoPrefferedSize : public core::System {
  std::shared_ptr<service::Camera> camera_service;
public:
  CameraAutoPrefferedSize(std::shared_ptr<service::Camera> camera_service):
    camera_service(std::move(camera_service))
  { }
  void setup(Settings& settings) const override {
    settings.priority = update_priority::CameraAutoPrefferedSize;
  }
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraAutoPrefferedSize>(
      entt::exclude<component::KeepCameraProportions>
    );

    view.each([&registry, this](auto entity, auto camera, const auto& auto_size) {
      sf::Vector2f render_size = camera_service->get_render_region(entity, registry).getSize();
      float combined_aspect_ratio = (render_size.x / render_size.y) / (auto_size.size_x / auto_size.size_y);
      if (combined_aspect_ratio > 1.0f) {
        camera.preferred_size_x = auto_size.size_x * combined_aspect_ratio;
        camera.preferred_size_y = auto_size.size_y;
      } else {
        camera.size_x = auto_size.size_x;
        camera.size_y = auto_size.size_y / combined_aspect_ratio;
      }
      registry.replace<component::Camera>(entity, camera);
    });
  }
};

CORE_DEFINE_SYSTEM("system::CameraAutoPrefferedSize", [](core::ServiceLocator& locator){
  return std::make_unique<CameraAutoPrefferedSize>(
    locator.get<service::Camera>()
  );
});



}
