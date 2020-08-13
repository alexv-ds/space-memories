#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <services/Camera.hpp>
#include <services/World.hpp>
#include <services/SpriteManager.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>
#include <algorithm>
#include <iostream>

namespace {

sf::Vector2f calculate_camera_offcet(const component::Camera& cam, const component::Position& pos) {
  return {
    ( (cam.size_x-1.0f)/-2 ) + pos.x,
    ( (cam.size_y-1.0f)/-2 ) + pos.y
  };
}

struct RenderElement {
  RenderElement(entt::entity entity,
                component::Position* p_position,
                component::Body* p_body,
                component::RenderableQuad* p_quad):
      //init
      entity(entity),
      p_position(p_position),
      p_body(p_body),
      p_quad(p_quad)
  {}

  entt::entity entity;
  component::Position* p_position;
  component::Body* p_body;
  component::RenderableQuad* p_quad;
};

inline bool render_queue_sort_func(const RenderElement& lhs, const RenderElement& rhs) {
  return lhs.p_position->layer < rhs.p_position->layer;
}

class QuadRenderSystem final : public core::System {
  std::shared_ptr<service::Camera> service_camera;
  std::shared_ptr<service::World> world;
  std::shared_ptr<service::SpriteManager> sprite_manager;
  entt::entity last_entity_camera = entt::null;
  sf::RenderTarget* last_render_target = nullptr;
  std::vector<entt::entity> query_buffer;
  std::vector<RenderElement> render_queue;
  sf::RectangleShape quad_shape;

public:
  QuadRenderSystem(std::shared_ptr<service::Camera> service_camera,
                   std::shared_ptr<service::World> world,
                   std::shared_ptr<service::SpriteManager> sprite_manager):
      service_camera(std::move(service_camera)),
      world(std::move(world)),
      sprite_manager(std::move(sprite_manager))
  {}
  void setup(Settings& settings) const override {
    settings.priority = update_priority::QuadRenderSystem;
  }

  void update(entt::registry& registry) override {
    registry.view<component::Camera, component::Position>().each([&, this](auto entity, auto& camera, auto& pos) {
      sf::RenderTarget* r_target = service_camera->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      query_buffer.clear();
      render_queue.clear();
      //Заполняем render_queue несортированными данными
      world->query_intersects_region(query_buffer, {pos.x, pos.y, camera.size_x, camera.size_y});
      for (entt::entity entity : query_buffer) {
        auto [p_position, p_body, p_quad] = registry.try_get<component::Position,
                                                             component::Body,
                                                             component::RenderableQuad>(entity);
        if (!p_position || !p_body || !p_quad) {
          continue;
        }
        render_queue.emplace_back(entity, p_position, p_body, p_quad);
      }
      //Сортируем render_queue по layer
      std::sort(render_queue.begin(), render_queue.end(), render_queue_sort_func);
      //Устанавливаем view
      sf::View old_view = r_target->getView();
      sf::View current_view = service_camera->calculate_camera_view(camera, old_view);
      current_view.move(calculate_camera_offcet(camera, pos));
      r_target->setView(current_view);
      //Рендерим
      for (const RenderElement& elem : render_queue) {
        quad_shape.setSize({elem.p_body->size_x, elem.p_body->size_y});
        quad_shape.setPosition(elem.p_position->x + (0.5f - elem.p_body->size_x / 2.0f),
                               elem.p_position->y + (0.5f - elem.p_body->size_y / 2.0f) );
        quad_shape.setFillColor(elem.p_quad->color);
        component::Sprite* p_sprite = registry.try_get<component::Sprite>(elem.entity);
        if (p_sprite) {
          sf::Texture* texture = sprite_manager->get_texture(p_sprite->id);
          if (texture) {
            quad_shape.setTexture(texture);
          }
        }
        r_target->draw(quad_shape);
      }
      r_target->setView(old_view);
    });




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

      sf::RectangleShape rect({1,1});

      rect.setFillColor(rquad.color);
      rect.setPosition({screen_pos.x, screen_pos.y});

      const sf::View old_render_view = last_render_target->getView();
      last_render_target->setView(service_camera->calculate_camera_view(*comp_camera, old_render_view));
      last_render_target->draw(rect);
      last_render_target->setView(old_render_view);
    });

  }

};


CORE_DEFINE_SYSTEM("system::QuadRenderSystem", [](core::ServiceLocator& locator){
  return std::make_unique<QuadRenderSystem>(
    locator.get<service::Camera>(),
    locator.get<service::World>(),
    locator.get<service::SpriteManager>()
  );
});

}
