#pragma once
#include <map>
#include <memory>
#include <core/Service.hpp>
#include <core/Logger.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <components/RenderWindow.hpp>
#include <entt/entity/entity.hpp>

namespace service {

class SFMLRenderWindow final : public core::Service {
public:
  SFMLRenderWindow(std::shared_ptr<core::Logger>);
  ~SFMLRenderWindow();
  void update_window(entt::entity, component::RenderWindow& window);
  void destroy_window(entt::entity);
  std::string_view impl_name() const noexcept override;
private:
  std::shared_ptr<core::Logger> logger;
  std::map<entt::entity,std::unique_ptr<sf::RenderWindow>> windows;
};

}