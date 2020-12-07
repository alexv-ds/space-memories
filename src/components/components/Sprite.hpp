#pragma once
#include <string>

namespace component {

struct Sprite {
  int icon = -1; //в json текст, имя файла
  int state = -1; //в json текст, имя стейта
  int dir = 0; //С++ only
};

struct SpaceIconInitialise {
  std::string dmi;
};

struct ForceSpriteFrame {
  int frame = 0;
};

}
