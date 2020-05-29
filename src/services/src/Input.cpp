#include <cassert>
#include <core/define_service.hpp>
#include <services/Input.hpp>

namespace service {

void Input::update_keys(entt::entity entity, const std::vector<sf::Event>& key_events) {
  KeyboardState& state = states[entity];
  for (const sf::Event& event : key_events) {
    assert(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased);
    if (event.type == sf::Event::KeyPressed) {
      state.set(static_cast<Key>(event.key.code), KeyboardState::DOWN|KeyboardState::PRESSED);
    } else {
      state.set(static_cast<Key>(event.key.code), KeyboardState::UP);
    }
  }
}

void Input::input_frame_end() {
  for(auto& [id, state] : states) {
    state.reset_all_down();
    state.reset_all_up();
  }
}

bool Input::is_key_down(entt::entity id, Key key) const {
  auto it = states.find(id);
  if (it == states.end()) {
    return false;
  }
  return it->second.get(key) & KeyboardState::DOWN;
}

bool Input::is_key_up(entt::entity id, Key key) const {
  auto it = states.find(id);
  if (it == states.end()) {
    return false;
  }
  return it->second.get(key) & KeyboardState::UP;
}

bool Input::is_key_pressed(entt::entity id, Key key) const {
  auto it = states.find(id);
  if (it == states.end()) {
    return false;
  }
  return it->second.get(key) & KeyboardState::PRESSED;
}

std::string_view Input::impl_name() const noexcept {
  return "service::Input";
}

CORE_DEFINE_SERVICE(Input, "service::Input", core::after(), [](core::ServiceLocator&){
  return std::make_shared<Input>();
});

}