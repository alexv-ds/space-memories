#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/Camera.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Camera.hpp>

namespace service {

//шоб компилёр не ругался, если поставить core::after напрямую, то выебуется
static auto build_after() {
  return core::after<service::SFMLRenderWindow, core::LoggerFactory>();
}

CORE_DEFINE_SERVICE(Camera,
                    "service::Camera",
                    build_after(),
  [](core::ServiceLocator& locator){
    auto logger_factory = locator.get<core::LoggerFactory>();
    auto sfml_render_widow = locator.get<service::SFMLRenderWindow>();
    return std::make_shared<Camera>(logger_factory->create_logger("service::Camera"), sfml_render_widow);
  }
);

Camera::Camera(std::shared_ptr<core::Logger> logger,
               std::shared_ptr<service::SFMLRenderWindow> render_window):
    logger(std::move(logger)),
    sfml_render_window(std::move(render_window))
{}

std::string_view Camera::impl_name() const noexcept {
  return "service::Camera";
}

sf::RenderTarget* Camera::get_render_target(entt::entity camera, const entt::registry& registry) {
  if(!registry.try_get<component::Camera>(camera)) {
    logger->warn("Невозможно получить sf::RenderTarget у entity-{}: отсутствует компонент Camera", camera);
    return nullptr;
  }
  const component::BindCameraToRenderWindow* bind_component =
    registry.try_get<component::BindCameraToRenderWindow>(camera);
  if (bind_component) {
    if (registry.valid(bind_component->window) && registry.has<component::RenderWindow>(bind_component->window)) {
      sf::RenderWindow* render_window = sfml_render_window->get_window(bind_component->window);
      if (!render_window) {
        logger->warn("Не удалось получить sf::RenderWindow");
      }
      return render_window;
    }
    return nullptr;
  }
  logger->warn("Невозможно получить sf::RenderTarget y entity-{}: камера ни к чему не привязана", camera);
  return nullptr;
}

void Camera::render_begin() {

}
void Camera::render_end() {

}

sf::FloatRect Camera::get_render_region(entt::entity entity, const entt::registry& registry) {
  sf::RenderTarget* target = get_render_target(entity, registry);
  const component::Camera* p_camera = registry.try_get<component::Camera>(entity);
  if (!target || !p_camera) {
    return {0.0f,0.0f,800.0f,600.0f};
  }
  sf::Vector2u size = target->getSize();
  sf::Vector2f f_size(static_cast<float>(size.x), static_cast<float>(size.y));

  const component::OverrideRenderRegion* p_region_override =
    registry.try_get<component::OverrideRenderRegion>(entity);
  if (p_region_override) {
    return p_region_override->rect;
  }

  if (registry.has<component::KeepCameraProportions>(entity)) {
    float combined_aspect_ratio = (f_size.x / f_size.y) / (p_camera->size_x / p_camera->size_y);
    if (combined_aspect_ratio > 1.0f) {
      float region_size_x = f_size.x / combined_aspect_ratio;
      sf::FloatRect region((f_size.x - region_size_x) * 0.5f, 0.0f, region_size_x, f_size.y);
      return region;
    } else if (combined_aspect_ratio < 1.0f) {
      float region_size_y = f_size.y * combined_aspect_ratio;
      sf::FloatRect region(0.0f, (f_size.y - region_size_y) * 0.5f, f_size.x, region_size_y);
      return region;
    } else {
      return {0.0f,0.0f, f_size.x, f_size.y};
    }
  }
  return {0.0f,0.0f, f_size.x, f_size.y};
}

}
