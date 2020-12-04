#include <core/define_system.hpp>
#include <components/Camera.hpp>

namespace {

class CameraAutoPrefferedSize : public core::System {
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraAutoPrefferedSize, component::CameraRenderRegion>(
      entt::exclude<component::KeepCameraProportions>
    );

    view.each([&registry, this](auto entity, auto camera, const auto& auto_size, const auto& render_region) {
      sf::Vector2f render_size = render_region.rect.getSize();
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
  return std::make_unique<CameraAutoPrefferedSize>();
});



}
