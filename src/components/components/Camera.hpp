#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <entt/entity/fwd.hpp>
#include "Position.hpp"

namespace component {

struct Camera {
  float size_x = 1.0;
  float size_y = 1.0;
  //Рекомендуемый размер, используется в системах которые подгоняют размер камеры по каким-то параметрам
  //например PixelPerfectCameraSize
  float preferred_size_x = 1.0f; //C++ only
  float preferred_size_y = 1.0f; //C++ only
};

//C++ only
struct BindCameraToRenderWindow {
  entt::entity window = entt::null;
};

//Рамка (границой наружу) вокруг области рендеринга камеры
struct DebugRenderRegionFrame {
  sf::Color color = sf::Color::White; //C++ only
};

struct CameraFixedUnitSize {
  float x = 32.0f;
  float y = 32.0f;
};

//Регион рендеринга в пикселях
//C++ only
struct CameraRenderRegion {
  sf::FloatRect rect{0.0f,0.0f,100.0f,100.0f};
};

struct OverrideRenderRegion {
  sf::FloatRect rect{0.0f,0.0f,100.0f,100.0f}; //в json объект с числовыми полями, left, top, width, height
};

}
