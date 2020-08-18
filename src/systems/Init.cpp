#include "system_update_priority.hpp"

#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <core/Logger.hpp>
#include <services/Input.hpp>
#include <services/Time.hpp>
#include <services/Camera.hpp>
#include <services/World.hpp>
#include <services/SpriteManager.hpp>
#include <components/RenderWindow.hpp>
#include <components/ExitAfterNSec.hpp>
#include <components/Input.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>
#include <components/Sprite.hpp>
#include <components/RenderMode.hpp>

namespace {

sf::Uint8 get_random_uint8() {
  return static_cast<sf::Uint8>(std::rand() % 256);
}

struct TestComponent {
  entt::entity sync_with = entt::null;
};

class Init final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Input> input;
  std::shared_ptr<service::Time> time;
  std::shared_ptr<service::Camera> camera_service;
  std::shared_ptr<service::World> world;
  std::shared_ptr<service::SpriteManager> sprite_manager;
  int counter = 360;
  bool delete_window = true;
public:
  Init(std::shared_ptr<core::Logger> logger,
       std::shared_ptr<service::Input> input,
       std::shared_ptr<service::Time> time,
       std::shared_ptr<service::Camera> camera_service,
       std::shared_ptr<service::World> world,
       std::shared_ptr<service::SpriteManager> sprite_manager):
    //init
    logger(logger),
    input(input),
    time(time),
    camera_service(camera_service),
    world(world),
    sprite_manager(sprite_manager)
  {}

  void setup(Settings& setting) const override {
    setting.priority = update_priority::Init;
  }

  void init(entt::registry& registry) override {
    entt::entity window = registry.create();
    registry.emplace<component::RenderWindow>(window);
    registry.emplace<component::ExitIfWindowClosed>(window);
    registry.emplace<component::ListenKeyboard>(window);
    entt::entity camera = registry.create();
    registry.emplace<component::Camera>(camera, 10.0f, 10.0f);
    registry.emplace<component::BindCameraToRenderWindow>(camera, window);
    registry.emplace<component::Position>(camera, 0.0f, 0.0f, 1.0f);
    registry.emplace<component::WASDRawInputMovable>(camera, window);
    registry.emplace<component::CameraFixedUnitSize>(camera, 64.0f, 64.0f);

    for (size_t x = 0; x < 30; ++x) {
      for (size_t y = 0; y < 30; ++y) {
        entt::entity entity = registry.create();
        registry.emplace<component::Position>(
          entity, static_cast<float>(x), static_cast<float>(y), 0.0f
        );
        registry.emplace<component::Body>(entity, 1.0f, 1.0f);
        registry.emplace<component::Sprite>(entity, sprite_manager->load_sprite("resources/floor.png"));
        registry.emplace<component::DefaultRenderMode>(entity);
      }
    }
    entt::entity entity = registry.create();
    registry.emplace<component::Position>(entity, 6.0f, 6.0f, 0.1f);
    registry.emplace<component::Body>(entity, 8.0f, 8.0f);
    registry.emplace<component::Sprite>(entity, sprite_manager->load_sprite("resources/floors.dmi"));
    registry.emplace<component::DefaultRenderMode>(entity);
  }

  void update(entt::registry& registry) override {

  }
};

CORE_DEFINE_SYSTEM("system::Init", [](core::ServiceLocator& locator){
  return std::make_unique<Init>(
    locator.get<core::LoggerFactory>()->create_logger("system::Init"),
    locator.get<service::Input>(),
    locator.get<service::Time>(),
    locator.get<service::Camera>(),
    locator.get<service::World>(),
    locator.get<service::SpriteManager>()
  );
});


}
