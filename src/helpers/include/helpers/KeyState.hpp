#include <SFML/Window/Keyboard.hpp>

namespace helpers {
  
struct KeyState {
  bool up = false;
  bool pressed = false;
  bool down = false;
};

}