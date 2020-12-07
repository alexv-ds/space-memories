#pragma once
#include <string>

namespace component {

struct SpriteOld {
  int icon = -1; //в json текст, имя файла
  int state = -1; //в json текст, имя стейта
  int dir = 0; //С++ only
};

struct ForceSpriteFrame {
  int frame = 0;
};

}
