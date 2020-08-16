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
#include <cmath>

namespace {

sf::Transform calculate_render_transform(const sf::FloatRect& region,
                                         component::Camera& camera,
                                         component::Position& camera_pos)
{
  sf::Transform transform;
  const sf::Vector2f region_size = region.getSize();
  const sf::Vector2f step_size(region_size.x / camera.size_x, region_size.y / camera.size_y);

  float translate_x = region.left + region.width * 0.5 - step_size.x * camera_pos.x - step_size.x * 0.5;
  float translate_y = region.top + region.height * 0.5 + step_size.y * camera_pos.y + step_size.y * 0.5;
  transform.translate(translate_x, translate_y);
  transform.scale(step_size.x,-step_size.y);
  return transform;
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

      sf::FloatRect render_region = service_camera->get_render_region(entity, registry);
      //resize_to_pixel_perfect(camera, render_region.getSize());
      sf::Transform render_transform = calculate_render_transform(render_region, camera, pos);
      //Рендерим
      for (const RenderElement& elem : render_queue) {
        sf::FloatRect rect(elem.p_position->x, elem.p_position->y, elem.p_body->size_x, elem.p_body->size_y);
        rect = render_transform.transformRect(rect);
        quad_shape.setSize({rect.width, rect.height});
        quad_shape.setPosition({rect.left, rect.top});
        quad_shape.setFillColor(elem.p_quad->color);
        component::Sprite* p_sprite = registry.try_get<component::Sprite>(elem.entity);
        if (p_sprite) {
          sf::Texture* texture = sprite_manager->get_texture(p_sprite->id);
          if (texture) {
            quad_shape.setTexture(texture);
          } else {
            quad_shape.setTexture(nullptr);
          }
        }
        r_target->draw(quad_shape);
      }
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
