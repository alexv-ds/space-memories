#include "TextureLoader.hpp"

#include <string>
#include <iostream>

namespace core::service {

TextureID TextureLoader::load(const std::string_view path) {
  if (path.size() < 1) {
    return {0};
  }

  std::string safe_string;
  size_t start_pos = 0;
  if (path[0] == '/') {
    start_pos = 1;
  }
  for (size_t i = start_pos; i < path.size(); ++i) {
    //Как-нибудь доделаю
    /*if (path[i] == '.') {
      continue;
    }*/
    safe_string.push_back(path[i]);
  }

  auto it_str_index = this->string_index.find(safe_string);
  if (it_str_index != this->string_index.end()) {
    return {it_str_index->second};
  }

  std::unique_ptr<sf::Texture> p_texture(new sf::Texture());
  bool load_result = p_texture->loadFromFile(safe_string);
  if (load_result == true) {
    bool generate_mipmap_result = p_texture->generateMipmap();
    std::cout << safe_string << " -- loaded" << '\n';
    if (generate_mipmap_result == false) {
      std::cout << safe_string << " -- fail to generate mipmap" << '\n';
    }
    int id = ++last_id;
    this->id_index[id] = std::move(p_texture);
    this->string_index[safe_string] = id;
    return {id};
  }
  return {0};
}

const sf::Texture* TextureLoader::get_texture(TextureID id) const {
  auto it = this->id_index.find(id.id);
  if (it != this->id_index.end()) {
    return it->second.get();
  }
  return nullptr;
}

} /* core::service */
