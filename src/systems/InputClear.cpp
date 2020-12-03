#include <core/define_system.hpp>
#include <components/Input.hpp>

namespace {

class InputClear : public core::System {
  std::vector<sf::Keyboard::Key> to_remove;
public:
  void update(entt::registry& registry) override {
    //С этим говнокодом определенно надо что-то сделать
    auto view = registry.view<component::ListenKeyboard>();
    for (entt::entity entity : view) {
      const auto& const_keyboard = view.get<component::ListenKeyboard>(entity);
      bool need_patch = false;
      for (auto& [key, state] : const_keyboard.keys) {
        if (state.up || state.down) {
          need_patch = true;
          break;
        }
      }
      if (!need_patch) {
        continue;
      }
      registry.patch<component::ListenKeyboard>(entity, [this](auto& keyboard) {
        to_remove.clear();
        for (auto& [key, state] : keyboard.keys) {
          if (state.up) {
            to_remove.push_back(key);
          } else {
            state.down = false;
          }
        }
        for (sf::Keyboard::Key key : to_remove) {
          keyboard.keys.erase(key);
        }
      });
    }
  }
};

CORE_DEFINE_SYSTEM("system::InputClear", [](core::ServiceLocator& locator) {
  return std::make_unique<InputClear>();
});

}