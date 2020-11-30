#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/SFMLRenderWindow.hpp>
#include <components/RenderWindow.hpp>
#include <imgui-SFML.h>

namespace service {

SFMLRenderWindow::SFMLRenderWindow(std::shared_ptr<core::Logger> logger): logger(std::move(logger)) {}

std::string_view SFMLRenderWindow::impl_name() const noexcept {
  return "service::SFMLRenderWindow";
}

SFMLRenderWindow::~SFMLRenderWindow() {
  windows.clear();
}

void SFMLRenderWindow::update_window(entt::entity entity) {
  auto it = windows.find(entity);
  if(it == windows.end()) {
    logger->debug("Открытие окна для entity-{}", entity);
    std::unique_ptr sf_window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(800u, 600u), "Space Memories"
    );
    //Первоначальная очистка, что бы пользователь
    //не видел мусор (разноцветный шум/части других окон/т.д) из видеобуффера
    sf_window->clear(sf::Color::Black);
    sf_window->display();

    sf_window->setKeyRepeatEnabled(false);
    
    ImGui::SFML::Init(*sf_window);
    windows[entity] = std::move(sf_window);
  }
}

void SFMLRenderWindow::destroy_window(entt::entity entity) {
  logger->debug("Уничтожение окна для entity-{}", entity);
  auto it = windows.find(entity);
  if (it == windows.end()) {
    logger->warn("Не удалось уничтожить окно для entity-{}, ассоциированное окно не найдено", entity);
    return;
  }
  ImGui::SFML::Init(*it->second);
  windows.erase(it);
}

sf::RenderWindow* SFMLRenderWindow::get_window(entt::entity entity) {
  auto it = windows.find(entity);
  if (it == windows.end()) {
    logger->warn("У entity-{} нет ассоциированного окна", entity);
    return nullptr;
  }
  return it->second.get();
}

CORE_DEFINE_SERVICE(SFMLRenderWindow, "service::SFMLRenderWindow", core::after<core::LoggerFactory>(),
  [](core::ServiceLocator& locator) {
    std::shared_ptr<core::LoggerFactory> logger_factory = locator.get<core::LoggerFactory>();
    return std::make_shared<SFMLRenderWindow>(logger_factory->create_logger("service::SFMLRenderWindow"));
  }
);

}
