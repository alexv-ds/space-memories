#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>

namespace {

sf::Vector2f calculate_cell_size(const component::Camera& cam, const sf::RenderTarget& rtarget) {
  const sf::View& render_view = rtarget.getView();
  sf::Vector2f cell_size = render_view.getSize();
  cell_size.x = cell_size.x / cam.size_x;
  cell_size.y = cell_size.y / cam.size_y;
  return cell_size;
}

class QuadRenderSystem final : public core::System {
  std::shared_ptr<service::Camera> service_camera;
  entt::entity last_entity_camera = entt::null;
  sf::RenderTarget* last_render_target = nullptr;
public:
  QuadRenderSystem(std::shared_ptr<service::Camera> service_camera): service_camera(std::move(service_camera)) {}
  void setup(Settings& settings) const override {
    settings.priority = update_priority::QuadRenderSystem;
  }
  void update(entt::registry& registry) override {
    last_entity_camera = entt::null;
    last_render_target = nullptr;
    auto view = registry.view<component::RenderableQuad, component::ScreenPosition>();
    view.each([this, &registry](auto entity, auto& rquad, auto& screen_pos) {
      if (!registry.valid(screen_pos.camera)) {
        return;
      }
      const component::Camera* comp_camera = registry.try_get<component::Camera>(screen_pos.camera);
      if (!comp_camera) {
        return;
      }
      if (last_entity_camera != entity) {
        last_entity_camera = entity;
        last_render_target = service_camera->get_render_target(screen_pos.camera, registry);
      }
      if (!last_render_target) {
        return;
      }

      sf::Vector2f cell_size = calculate_cell_size(*comp_camera, *last_render_target);
      sf::RectangleShape rect({cell_size.x,cell_size.y});
      rect.setFillColor(rquad.color);
      rect.setPosition({cell_size.x*screen_pos.x, cell_size.y*screen_pos.y});

      const sf::View old_render_view = last_render_target->getView();
      sf::View render_view = old_render_view;
      const sf::Vector2f& render_view_size = old_render_view.getSize(); 
      render_view.setSize(render_view_size.x, -render_view_size.y);
      last_render_target->setView(render_view);
      last_render_target->draw(rect);
      last_render_target->setView(old_render_view);
    });
    
  }

};


CORE_DEFINE_SYSTEM("system::QuadRenderSystem", [](core::ServiceLocator& locator){
  return std::make_unique<QuadRenderSystem>(locator.get<service::Camera>());
});

}