#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <map>
#include <algorithm>

struct State {
  std::string name;
  std::vector<float> delays;
  std::vector<std::vector<sf::IntRect>> dirs;
};



struct SpriteSheet {
  sf::Texture texture;
  std::vector<State> states;

  int find(std::string_view state_name) {
    //временное решение
    for (size_t i = 0; i < states.size(); ++i) {
      if (state_name == states[i].name) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }
};
