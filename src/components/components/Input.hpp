#pragma once
#include <entt/entity/fwd.hpp>
#include <SFML/Window/Event.hpp>
#include <helpers/KeyState.hpp>
#include <vector>
#include <map>

namespace component {

//Работает в связке с component::ListenWindowEvents
struct ListenKeyboard {
  std::map<sf::Keyboard::Key, helpers::KeyState> keys;
  
  //help funcs
  inline bool is_key_up(sf::Keyboard::Key key) const {
    auto it = keys.find(key);
    return it != keys.end() && it->second.up;
  }
  inline bool is_key_down(sf::Keyboard::Key key) const {
    auto it = keys.find(key);
    return it != keys.end() && it->second.down;
  }
  inline bool is_key_pressed(sf::Keyboard::Key key) const {
    auto it = keys.find(key);
    return it != keys.end() && it->second.pressed;
  }
};

//Вешается на энтитю с component::RenderWindow
struct ListenWindowEvents {
  //Или std::list + pmr лучше, хуй знает.
  std::vector<sf::Event> events; //C++ only
};

//C++ only
struct CopyWindowEvents {
  entt::entity target = entt::null;
};

//Костыль для теста
//C++ only
struct WASDRawInputMovable {
  entt::entity input_listener = entt::null;
};

}
