#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/RenderWindow.hpp>

namespace service {

SFMLRenderWindow::SFMLRenderWindow(std::shared_ptr<core::Logger> logger): logger(std::move(logger)) {}

std::string_view SFMLRenderWindow::impl_name() const noexcept {
  return "service::SFMLRenderWindow";
}

void SFMLRenderWindow::update_window(entt::entity entity, component::RenderWindow& comp_window) {
  if(auto it = windows.find(entity); it == windows.end()) {
    logger->debug("Открытие окна для entity-{}", entity);
    std::unique_ptr sf_window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode::getDesktopMode(), "SFML WIndow"
    );
    windows[entity] = std::move(sf_window);
  }
}
SFMLRenderWindow::~SFMLRenderWindow() {
  windows.clear();
}

void SFMLRenderWindow::destroy_window(entt::entity entity) {
  logger->debug("Уничтожение окна для entity-{}", entity);
  auto it = windows.find(entity);
  if (it == windows.end()) {
    logger->warn("Не удалось уничтожить окно для entity-{}, ассоциированное окно не найдено", entity);
    return;
  }
  windows.erase(it);
}

CORE_DEFINE_SERVICE(SFMLRenderWindow, "service::SFMLRenderWindow", core::after<core::LoggerFactory>(),
  [](core::ServiceLocator& locator) {
    std::shared_ptr<core::LoggerFactory> logger_factory = locator.get<core::LoggerFactory>();
    return std::make_shared<SFMLRenderWindow>(logger_factory->create_logger("service::SFMLRenderWindow"));
  }
);

}