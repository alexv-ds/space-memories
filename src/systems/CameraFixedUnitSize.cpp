#include <core/define_system.hpp>
#include <components/Camera.hpp>

namespace {

class CameraFixedUnitSize : public core::System {
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::CameraFixedUnitSize, component::CameraRenderRegion>();

    view.each([&registry, this](auto entity, auto camera, const auto& unit_size, const auto& render_region) {
      sf::Vector2f render_size = render_region.rect.getSize();
      camera.size_x = render_size.x / unit_size.x;
      camera.size_y = render_size.y / unit_size.y;
      registry.replace<component::Camera>(entity, camera);
    });
  }
};

CORE_DEFINE_SYSTEM("system::CameraFixedUnitSize", [](core::ServiceLocator& locator){
  return std::make_unique<CameraFixedUnitSize>();
});


}
