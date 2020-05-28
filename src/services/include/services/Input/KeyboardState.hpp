#pragma once

#include <bitset>
#include "Key.hpp"
#include <iostream>
class KeyboardState {
public:
  static constexpr int EMPTY = 0;
  static constexpr int UP = (1<<0);
  static constexpr int DOWN = (1<<1);
  static constexpr int PRESSED = (1<<2);

  inline int  get(Key key) const;
  inline void set(Key key, int state);
  inline void reset_all_up();
  inline void reset_all_down();
  inline void reset_all_pressed();

private:
  std::bitset<static_cast<size_t>(Key::KeyCount)> keys_up;
  std::bitset<static_cast<size_t>(Key::KeyCount)> keys_down;
  std::bitset<static_cast<size_t>(Key::KeyCount)> keys_pressed;
};


inline int KeyboardState::get(Key key) const {
  if (key == Key::Unknown) {
    return 0;
  }
  int state = 0;
  if (keys_up[static_cast<size_t>(key)]) {
    state |= UP;
  }
  if (keys_down[static_cast<size_t>(key)]) {
    state |= DOWN;
  }
  if (keys_pressed[static_cast<size_t>(key)]) {
    state |= PRESSED;
  }
  return state;
}

inline void KeyboardState::set(Key key, int state) {
  if (key == Key::Unknown) {
    return;
  }
  keys_up[static_cast<size_t>(key)]      = state & UP ? true : false;
  keys_down[static_cast<size_t>(key)]    = state & DOWN ? true : false;
  keys_pressed[static_cast<size_t>(key)] = state & PRESSED ? true : false;
}

inline void KeyboardState::reset_all_up() {
  keys_up = std::bitset<static_cast<size_t>(Key::KeyCount)>();
}

inline void KeyboardState::reset_all_down() {
  keys_down = std::bitset<static_cast<size_t>(Key::KeyCount)>();
}

inline void KeyboardState::reset_all_pressed() {
  keys_pressed = std::bitset<static_cast<size_t>(Key::KeyCount)>();
}


