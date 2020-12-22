#pragma once
#include <string>
#include <memory>
#include <vector>
#include <SFML/Graphics/Texture.hpp>

namespace component {

struct Sprite {
  std::string icon;
  std::string state;
};

//C++ only
struct Texture {
  std::shared_ptr<const sf::Texture> texture;
};

//Если присутствует, то регион текстуры будет браться отсюда
//Имеет приоритет выше чем TextureAutoAnimation
//C++ only
struct TextureRegion {
  sf::IntRect region;
};

//Если присутствует, то регион текстуры будет браться отсюда
//Взависимости от текущего времени
//Приоритет ниже чем TextureRegion
//C++ only
struct TextureAutoAnimation {
  struct Frame {
    float time = 0.0f;
    sf::IntRect region;
  };
  std::vector<Frame> frames;
  float full_time = 0.0f;
};

}
