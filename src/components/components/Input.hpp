#pragma once
#include <entt/entity/fwd.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>


namespace component {

//Вешается на энтитю с component::RenderWindow
struct ListenKeyboard {};

//Вешается на энтитю с component::RenderWindow
struct ListenWindowEvents {
  //Или std::list + pmr лучше, хуй знает.
  std::vector<sf::Event> events; //C++ only
};

//Костыль для теста
//C++ only
struct WASDRawInputMovable {
  entt::entity input_listener = entt::null;
};

}
