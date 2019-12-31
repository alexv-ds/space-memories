#pragma once
#include "service_helpers.hpp"

#include <unordered_map>
#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <core/TextureID.hpp>

namespace core::service {

class TextureLoader {
public:
  static constexpr const std::string_view name = "core::service::TextureLoader";
  TextureID load(const std::string_view path);
  const sf::Texture* get_texture(TextureID id) const;
private:
  std::unordered_map<int, std::unique_ptr<sf::Texture>> id_index;
  std::unordered_map<std::string, int> string_index;
  int last_id = 0;
};

}
