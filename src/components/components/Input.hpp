#pragma once
#include <entt/entity/entity.hpp>

namespace component {

struct ListenKeyboard {};
//Костыль для теста
struct WASDRawInputMovable {
  entt::entity input_listener = entt::null;  
};

}