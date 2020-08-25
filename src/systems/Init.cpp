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
#include <components/Map.hpp>

namespace {



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
    registry.emplace<component::Position>(camera, 3.0f, 3.0f, 1.0f);
    registry.emplace<component::WASDRawInputMovable>(camera, window);
    registry.emplace<component::CameraFixedUnitSize>(camera, 64.0f, 64.0f);
    registry.emplace<component::LoadMap>(camera, "maps/room.json");
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
