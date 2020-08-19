#pragma once

namespace component {

struct Sprite {
  int icon = -1;
  int state = -1;
  int dir = 0;
  int frame = 0;
};

struct SpriteFrameAnimation {
  int icon_cache = -1;
  int state_cache = -1;
  float delay = 0.0f; //time
  bool no_animation = false;
};

}
