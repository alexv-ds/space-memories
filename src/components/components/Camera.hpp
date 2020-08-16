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

//С этим компонентом камера будет по центру рендер окна(рендер таргета)
//И не будет искажаться при изменении размеров окна
//Этот компомент учитывается в сервисе service::Camera
//При запросе области рендера окна
struct KeepCameraProportions {};

//Рамка (границой наружу) вокруг области рендеринга камеры
struct DebugRenderRegionFrame {
  sf::Color color = sf::Color::White;
};

//изменяет рекомендуемые размеры камеры так, что бы не было искажений
//но нужно указать рекомендуемые размеры для рекомендуемых размеров
//лол
struct CameraAutoPrefferedSize {
  float size_x = 1.0f;
  float size_y = 1.0f;
};
}
