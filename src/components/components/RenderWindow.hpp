#pragma once
#include <entt/entity/fwd.hpp>
#include <entt/entity/entity.hpp>

namespace component {

struct RenderWindow {};

//Вешается в Init, временно, пока я не придумаю лучшего способа связывать энтити друг с другом
//C++ only
struct MainCameraLink {
  entt::entity camera = entt::null;
};

//Выход из программы, если будет найден этот компонент без RenderWindow
struct ExitIfWindowClosed {};

}
