#pragma once
#include <entt/entity/entity.hpp>

namespace component {

//Вешается на энтитю с component::RenderWindow
struct ListenKeyboard {};

//Костыль для теста
//C++ only
struct WASDRawInputMovable {
  entt::entity input_listener = entt::null;
};

}
