#pragma once

#include <SFML/Graphics/Color.hpp>
#include <entt/entity/entity.hpp>
#include "Position.hpp"

namespace component {

struct Camera {
  float size_x = 1.0;
  float size_y = 1.0;
  //Рекомендуемый размер, используется в системах которые подгоняют размер камеры по каким-то параметрам
  //например PixelPerfectCameraSize
  float preferred_size_x = 1.0f;
  float preferred_size_y = 1.0f;
};

//Немного изменяет размеры камеры, что бы не было артефатов при рисовании спрайтов
//Это особенно заменто при рисовании одинаковых (да и не только) спрайтов рядом друг с другом
//таких как плитка пола
struct PixelPerfectCameraSize {
  int multiple_of = 32;
};

struct BindCameraToRenderWindow {
  entt::entity window = entt::null;
};

//Черные полосы по бокам (рисуется за областью камеры что бы перекрыть вылезающие изображения)
//работает в связке с BindCameraToRenderWindow
struct CameraPillarbox {};

struct RenderableQuad {
  sf::Color color = sf::Color::White;
};

}
