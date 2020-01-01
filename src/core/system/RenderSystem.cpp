#include "RenderSystem.hpp"

#include <core/component/Position.hpp>
#include <core/component/Renderable.hpp>
#include <core/component/Texture.hpp>

namespace core::system {

const std::string_view RenderSystem::name() const noexcept {
  return "core::system::RenderSystem";
}

RenderSystem::RenderSystem() {
  this->window = service::get_service<service::Window>();
  this->texture_loader = service::get_service<service::TextureLoader>();

}

void RenderSystem::update() {
  sf::RenderWindow* render_window = this->window->get_render_window();
  if (!render_window) {
    return;
  }
  const sf::Vector2u window_size = render_window->getSize();
  sf::View window_view(sf::FloatRect(0,0, (window_size.x*14)/window_size.y, 14));
  window_view.setCenter(7,-7);
  render_window->setView(window_view);



  auto view = this->registry->view<
    core::component::Renderable,
    core::component::Position,
    core::component::Texture
  >();

  for(entt::entity entity : view) {
    auto& com_renderable = this->registry->get<core::component::Renderable>(entity);
    auto& com_position = this->registry->get<core::component::Position>(entity);
    auto& com_texture = this->registry->get<core::component::Texture>(entity);

    const sf::Texture* texture = this->texture_loader->get_texture(com_texture.id);
    if (!texture) {
      break;
    }
    sf::Vertex vertices[4];
    sf::Vector2u texture_size = texture->getSize();

    //fill texture coords
    vertices[0].texCoords = sf::Vector2f(0,0);
    vertices[1].texCoords = sf::Vector2f(0, texture_size.y);
    vertices[2].texCoords = sf::Vector2f(texture_size.x, 0);
    vertices[3].texCoords = sf::Vector2f(texture_size.x, texture_size.y);

    //fill position coords
    vertices[0].position = sf::Vector2f(com_position.x,-(com_position.y+com_renderable.size_y));
    vertices[1].position = sf::Vector2f(com_position.x,-com_position.y);
    vertices[2].position = sf::Vector2f(com_position.x+com_renderable.size_x,-(com_position.y+com_renderable.size_y));
    vertices[3].position = sf::Vector2f(com_position.x+com_renderable.size_x,-com_position.y);

    render_window->draw(vertices, 4, sf::TriangleStrip, sf::RenderStates(texture));
  }
  const sf::View restored_view(sf::FloatRect(0,0, window_size.x, window_size.y));
  render_window->setView(restored_view);
}

} /* core::system */
