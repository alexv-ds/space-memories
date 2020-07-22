#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <services/Camera.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>
#include <iostream>

namespace {

sf::Vector2f calculate_cell_size(const component::Camera& cam, const sf::RenderTarget& rtarget) {
  const sf::View& render_view = rtarget.getView();
  sf::Vector2f cell_size = render_view.getSize();
  cell_size.x = cell_size.x / cam.size_x;
  cell_size.y = cell_size.y / cam.size_y;
  return cell_size;
}

sf::Vector2f vec2u_to_vec2f(const sf::Vector2u& vec2u) {
  return {static_cast<float>(vec2u.x), static_cast<float>(vec2u.y)};
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
      sf::RectangleShape rect({1,1});
      rect.setFillColor(rquad.color);
      rect.setPosition({screen_pos.x, screen_pos.y});

      const sf::View old_render_view = last_render_target->getView();
      sf::View render_view = old_render_view;
      const sf::Vector2f& render_view_size = old_render_view.getSize(); 

      
      float vsize_x;
      float vsize_y;
      const sf::Vector2f rtarget_size = vec2u_to_vec2f(last_render_target->getSize());
      float rtarget_aspect_ratio = rtarget_size.x / rtarget_size.y;
      float camera_aspect_ratio = comp_camera->size_x / comp_camera->size_y;
      float combined_aspect_ratio = rtarget_aspect_ratio / camera_aspect_ratio;
      
      if (combined_aspect_ratio > 1.0f) {
        vsize_x = comp_camera->size_x * combined_aspect_ratio;
        vsize_y = comp_camera->size_y;
        render_view.setViewport({(1.0f - 1.0f / combined_aspect_ratio) / 2.0f, 0.0f, 1.0f, 1.0f});
      } else {
        vsize_x = comp_camera->size_x;
        vsize_y = comp_camera->size_y / combined_aspect_ratio;
        render_view.setViewport({0.0f, -(1.0f - 1.0f * combined_aspect_ratio) / 2.0f, 1.0f, 1.0f});    
      }

      render_view.setSize(vsize_x, -vsize_y);
      render_view.setCenter(vsize_x / 2.0f, vsize_y / 2.0f);
      
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