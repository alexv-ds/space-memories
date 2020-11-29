#pragma once
#include <string>

namespace component {

struct Sprite {
  int icon = -1; //в json текст, имя файла
  int state = -1; //в json текст, имя стейта
  int dir = 0; //С++ only
  int frame = 0; //C++ only
};

struct SpriteFrameAnimation {
  int icon_cache = -1; //C++ only
  int state_cache = -1; //C++ only
  float delay = 0.0f; //C++ only
  bool no_animation = false; //C++ only
};

//Вешает на энтитю SpriteFrameAnimation, если у него 
//Спрайт с анимацией. Короткоживущий компонент.
struct AddSpriteFrameeAnimationIfNeeded {};

struct SpaceIconInitialise {
  std::string dmi;
};

}
