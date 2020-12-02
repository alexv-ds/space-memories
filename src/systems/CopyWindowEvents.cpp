#include <core/define_system.hpp>
#include <components/Input.hpp>
#include <components/Position.hpp>
#include <algorithm>

namespace {

class CopyWindowEvents : public core::System {
public:
  void update(entt::registry& registry) override {
    auto view = registry.view<component::CopyWindowEvents>();
    view.each([this, &registry](auto entity, const auto& copy_events) {
      if (registry.valid(copy_events.target) && registry.has<component::ListenWindowEvents>(copy_events.target)) {
        const auto& from = registry.get<component::ListenWindowEvents>(copy_events.target);
        if (registry.has<component::ListenWindowEvents>(entity)) {
          registry.patch<component::ListenWindowEvents>(entity, [&from](auto& to) {
            std::copy(from.events.begin(), from.events.end(), std::back_inserter(to.events));
          });
        } else {
          registry.emplace<component::ListenWindowEvents>(entity, from);
        }
      }
    });
  }
};


CORE_DEFINE_SYSTEM("system::CopyWindowEvents", [](core::ServiceLocator& locator) {
  return std::make_unique<CopyWindowEvents>();
});

}