#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <core/Process.hpp>
#include <services/PrototypeBuilder.hpp>
#include <components/RenderWindow.hpp>
#include <components/Input.hpp>
#include <components/Camera.hpp>

namespace {

class Init final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::PrototypeBuilder> prototype_builder;
  std::shared_ptr<core::Process> process;
public:
  Init(std::shared_ptr<core::Logger> logger,
       std::shared_ptr<service::PrototypeBuilder> prototype_builder,
       std::shared_ptr<core::Process> process):
    //init
    logger(logger),
    prototype_builder(prototype_builder),
    process(process)
  {}

  void init(entt::registry& registry) override {
    entt::entity window = prototype_builder->build("main_window", registry);
    if (window == entt::null) {
      logger->critical("Не удалось создать главное окно");
      process->exit("Ошибка инициализации");
      return;
    }
    entt::entity camera = prototype_builder->build("main_camera", registry);
    if (camera == entt::null) {
      logger->critical("Не удалось создать главную камеру");
      process->exit("Ошибка инициализации");
      return;
    }
    registry.emplace_or_replace<component::WASDRawInputMovable>(camera, window);
    registry.emplace_or_replace<component::BindCameraToRenderWindow>(camera, window);
    registry.emplace_or_replace<component::CopyWindowEvents>(camera, window);
    
    registry.emplace_or_replace<component::MainCameraLink>(window, camera);
    
    for (int x = -10; x < 50; ++x) {
      for (int y = -10; y < 50; ++y) {
        entt::entity space = prototype_builder->build("turf/space", registry);
        if (space == entt::null) {
          continue;
        }
        registry.emplace_or_replace<component::Position>(space, (float)x, (float)y, -1.0f);
      }
    }
  }

  void update(entt::registry& registry) override {

  }
};

CORE_DEFINE_SYSTEM("system::Init", [](core::ServiceLocator& locator){
  return std::make_unique<Init>(
    locator.get<core::LoggerFactory>()->create_logger("system::Init"),
    locator.get<service::PrototypeBuilder>(),
    locator.get<core::Process>()
  );
});


}
