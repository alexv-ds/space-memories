#include <core/define_system.hpp>
#include <core/Process.hpp>
#include <components/RenderWindow.hpp>

namespace {

class ExitIfWindowClosed final : public core::System {
  std::shared_ptr<core::Process> process;
  std::string msg;
public:
  ExitIfWindowClosed(std::shared_ptr<core::Process> process): process(process) {}
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ExitIfWindowClosed>(entt::exclude<component::RenderWindow>);
    for (entt::entity entity : view) {
      registry.remove<component::ExitIfWindowClosed>(entity);
      process->exit("Запрос от system::ExitIfWindowClosed");
    }
  }

};

CORE_DEFINE_SYSTEM("system::ExitIfWindowClosed", [](core::ServiceLocator& locator) {
  return std::make_unique<ExitIfWindowClosed>(locator.get<core::Process>());
});


}