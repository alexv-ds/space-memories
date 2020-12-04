#include <core/Service.hpp>
#include <entt/entity/registry.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/Camera.hpp>

namespace service {

class Camera final : public core::Service {
public:
  sf::RenderTarget* get_render_target(entt::entity camera, const entt::registry& registry); //can return nullptr

  Camera(std::shared_ptr<core::Logger>, std::shared_ptr<service::SFMLRenderWindow>);
  std::string_view impl_name() const noexcept override;
  void render_begin();
  void render_end();
private:
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::SFMLRenderWindow> sfml_render_window;
  std::map<entt::entity, std::unique_ptr<sf::RenderTexture>> render_textures;
};


}
