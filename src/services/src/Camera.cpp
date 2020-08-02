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
    if (registry.valid(bind_component->window) && registry.try_get<component::RenderWindow>(bind_component->window)) {
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

sf::View Camera::calculate_camera_view(const component::Camera& camera, const sf::View& oldview) {
  sf::View newview = oldview;
  float vsize_x;
  float vsize_y;
  const sf::Vector2f rtarget_size = oldview.getSize();
  float rtarget_aspect_ratio = rtarget_size.x / rtarget_size.y;
  float camera_aspect_ratio = camera.size_x / camera.size_y;
  float combined_aspect_ratio = rtarget_aspect_ratio / camera_aspect_ratio;
  if (combined_aspect_ratio > 1.0f) {
    vsize_x = camera.size_x * combined_aspect_ratio;
    vsize_y = camera.size_y;
    newview.setViewport({(1.0f - 1.0f / combined_aspect_ratio) / 2.0f, 0.0f, 1.0f, 1.0f});
  } else {
    vsize_x = camera.size_x;
    vsize_y = camera.size_y / combined_aspect_ratio;
    newview.setViewport({0.0f, -(1.0f - 1.0f * combined_aspect_ratio) / 2.0f, 1.0f, 1.0f});    
  }
  newview.setSize(vsize_x, -vsize_y);
  newview.setCenter(vsize_x / 2.0f, vsize_y / 2.0f);
  return newview;
}

}