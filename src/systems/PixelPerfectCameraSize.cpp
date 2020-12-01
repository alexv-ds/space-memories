#include <core/define_system.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>

namespace {

class PixelPerfectCameraSize : public core::System {
  std::shared_ptr<service::Camera> camera_service;
public:
  PixelPerfectCameraSize(std::shared_ptr<service::Camera> camera_service):
    camera_service(std::move(camera_service))
  {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::PixelPerfectCameraSize>();
    view.each([&registry, this](auto entity, auto camera, const auto& pixel_perfect) {
      const sf::Vector2f draw_size = camera_service->get_render_region(entity, registry).getSize();
      int step_x = static_cast<int>(std::round(draw_size.x / camera.preferred_size_x));
      int step_y = static_cast<int>(std::round(draw_size.y / camera.preferred_size_y));
      step_x -= step_x % pixel_perfect.multiple_of;
      step_y -= step_y % pixel_perfect.multiple_of;
      if (step_x == 0 || step_y == 0) {
        //лучше артефакты при рендере чем деление на 0
        camera.size_x = camera.preferred_size_x;
        camera.size_y = camera.preferred_size_y;
      } else {
        camera.size_x = draw_size.x / static_cast<float>(step_x);
        camera.size_y = draw_size.y / static_cast<float>(step_y);
      }
      registry.replace<component::Camera>(entity, camera);
    });
  }
};


CORE_DEFINE_SYSTEM("system::PixelPerfectCameraSize", [](core::ServiceLocator& locator){
  return std::make_unique<PixelPerfectCameraSize>(
    locator.get<service::Camera>()
  );
});



}