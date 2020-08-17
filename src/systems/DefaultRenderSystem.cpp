#include "system_update_priority.hpp"
#include <core/define_system.hpp>
#include <services/Camera.hpp>
#include <services/World.hpp>
#include <services/SpriteManager.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>
#include <components/RenderMode.hpp>
#include <components/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <iostream>
namespace {

struct RenderElement {
  RenderElement(entt::entity entity,
                const component::Position* p_position,
                const component::Body* p_body,
                const component::DefaultRenderMode* p_render_mode):
      //init
      entity(entity),
      p_position(p_position),
      p_body(p_body),
      p_render_mode(p_render_mode)
  {}

  entt::entity entity;
  const component::Position* p_position;
  const component::Body* p_body;
  const component::DefaultRenderMode* p_render_mode;
};

inline bool render_queue_sort_func(const RenderElement& lhs, const RenderElement& rhs) {
  return lhs.p_position->layer < rhs.p_position->layer;
}

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


class DefaultRenderSystem : public core::System {
  std::shared_ptr<service::Camera> camera_service;
  std::shared_ptr<service::World> world;
  std::shared_ptr<service::SpriteManager> sprite_manager;
  std::vector<entt::entity> world_query_buffer;
  std::vector<RenderElement> render_queue;
  sf::RectangleShape shape;
public:
  DefaultRenderSystem(std::shared_ptr<service::Camera> camera_service,
                      std::shared_ptr<service::World> world,
                      std::shared_ptr<service::SpriteManager> sprite_manager):
    camera_service(std::move(camera_service)),
    world(std::move(world)),
    sprite_manager(std::move(sprite_manager))
  {}

  void setup(Settings& settings) const override {
    settings.priority = update_priority::DefaultRenderSystem;
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::Camera, component::Position>();
    view.each([this, &registry](auto entity, auto& camera, auto& position) {
      sf::RenderTarget* r_target = camera_service->get_render_target(entity, registry);
      if (!r_target) {
        return;
      }
      world_query_buffer.clear();
      render_queue.clear();
      world->query_intersects_region(world_query_buffer, {position.x, position.y, camera.size_x, camera.size_y});
      fill_render_queue(registry);

      sf::FloatRect render_region = camera_service->get_render_region(entity, registry);
      sf::Transform render_transform = calculate_render_transform(render_region, camera, position);
      for (const RenderElement& elem : render_queue) {
        sf::FloatRect rect(
          elem.p_position->x + (0.5f - elem.p_body->size_x / 2.0f),
          elem.p_position->y + (0.5f - elem.p_body->size_y / 2.0f),
          elem.p_body->size_x,
          elem.p_body->size_y
        );
        rect = render_transform.transformRect(rect);
        shape.setSize({rect.width, rect.height});
        shape.setPosition({rect.left, rect.top});
        shape.setFillColor(elem.p_render_mode->color);
        const component::Sprite* p_sprite = registry.try_get<component::Sprite>(elem.entity);
        if (p_sprite) {
          auto [texture, texture_rect] = sprite_manager->get_texture(*p_sprite);
          if (texture) {
            shape.setTexture(texture);
            shape.setTextureRect(texture_rect);
          }
        }
        r_target->draw(shape, {elem.p_render_mode->blend_mode});
        shape.setTexture(nullptr);
      }
    });
  }

  void fill_render_queue(const entt::registry& registry) {
    for (entt::entity entity : world_query_buffer) {
      auto [p_position, p_body, p_render_mode] = registry.try_get<component::Position, component::Body, component::DefaultRenderMode>(entity);
      if (p_position && p_body && p_render_mode) {
        render_queue.emplace_back(entity, p_position, p_body, p_render_mode);
      }
    }
    std::sort(render_queue.begin(), render_queue.end(), render_queue_sort_func);
  }


};


CORE_DEFINE_SYSTEM("system::DefaultRenderSystem", [](core::ServiceLocator& locator){
  return std::make_unique<DefaultRenderSystem>(
    locator.get<service::Camera>(),
    locator.get<service::World>(),
    locator.get<service::SpriteManager>()
  );
});

}
