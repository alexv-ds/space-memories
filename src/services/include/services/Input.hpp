#pragma once
#include <vector>
#include <map>
#include <core/Service.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entity/entity.hpp>
#include "Input/Key.hpp"
#include "Input/KeyboardState.hpp"


namespace service {

class Input final : public core::Service {
public:
  bool is_key_down(entt::entity id, Key key) const;
  bool is_key_up(entt::entity id, Key key) const;
  bool is_key_pressed(entt::entity id, Key key) const;

  void update_keys(entt::entity id, const std::vector<sf::Event>& key_events);
  std::string_view impl_name() const noexcept override;
private:
  std::map<entt::entity, KeyboardState> states;
};

};