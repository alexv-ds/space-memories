#include <core/define_system.hpp>
#include <components/Input.hpp>

namespace {

class ListenWindowEventsClear final : public core::System {
  float speed = 0.1f;
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::ListenWindowEvents>();
    view.each([&, this](auto entity, const auto&) {
      registry.patch<component::ListenWindowEvents>(entity, [](auto& listen) {
        listen.events.clear();
      });
    });
  }
};

CORE_DEFINE_SYSTEM("system::ListenWindowEventsClear", [](core::ServiceLocator& locator) {
  return std::make_unique<ListenWindowEventsClear>();
});

}