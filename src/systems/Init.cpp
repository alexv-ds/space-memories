#include "system_update_priority.hpp"

#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <core/Logger.hpp>
#include <services/Input.hpp>
#include <services/Time.hpp>
#include <services/Camera.hpp>
#include <services/World.hpp>
#include <components/RenderWindow.hpp>
#include <components/ExitAfterNSec.hpp>
#include <components/Input.hpp>
#include <components/Camera.hpp>
#include <components/Position.hpp>


#include <SFML/Graphics.hpp>

namespace {

class Init final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Input> input;
  std::shared_ptr<service::Time> time;
  std::shared_ptr<service::Camera> camera_service;
  std::shared_ptr<service::World> world;
  int counter = 360;
  bool delete_window = true;
public:
  Init(std::shared_ptr<core::Logger> logger,
       std::shared_ptr<service::Input> input,
       std::shared_ptr<service::Time> time,
       std::shared_ptr<service::Camera> camera_service,
       std::shared_ptr<service::World> world
       ):
    //init
    logger(logger),
    input(input),
    time(time),
    camera_service(camera_service),
    world(world)
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

    for (size_t x = 0; x < 10; ++x) {
      for (size_t y = 0; y < 10; ++y) {
        entt::entity entity = registry.create();
        registry.emplace<component::Position>(entity, static_cast<float>(x), static_cast<float>(y));
        registry.emplace<component::Body>(entity);
      }
    }
  }

  void update(entt::registry& registry) override {
    auto view = registry.view<component::RenderableQuad, component::ScreenPosition>();
    view.each([&](auto entity, auto& comp_quad, auto& comp_screen_pos) {
      if (registry.valid(comp_screen_pos.camera)) {
        auto* camera_component = registry.try_get<component::Camera>(comp_screen_pos.camera);
        if (!camera_component) {
          return;
        }
        comp_screen_pos.x += 1.0f;
        if (comp_screen_pos.x >= camera_component->size_x) {
          comp_screen_pos.x = 0.0f;
          comp_screen_pos.y += 1.0f;
          if (comp_screen_pos.y >= camera_component->size_y) {
            comp_screen_pos.y = 0.0f;
          }
        }
      }
    });
    
    static thread_local bool print = true;
    if (!print) {
      return;
    }
    print = false;
    std::vector<entt::entity> entities;
    world->query_intersects_region(entities, {5.0f, 5.0f, 1.0f, 1.0f});

    for (entt::entity entity : entities) {
      const auto& comp_pos = registry.get<component::Position>(entity);
      logger->trace("entity-{}, x: {}, y: {}", entity, comp_pos.x, comp_pos.y);
    }
    logger->trace("Количество: {}", entities.size());
  }
};

CORE_DEFINE_SYSTEM("system::Init", [](core::ServiceLocator& locator){
  return std::make_unique<Init>(
    locator.get<core::LoggerFactory>()->create_logger("system::Init"),
    locator.get<service::Input>(),
    locator.get<service::Time>(),
    locator.get<service::Camera>(),
    locator.get<service::World>()
  );
});


}