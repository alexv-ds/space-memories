#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/Time.hpp>
#include <services/SpriteManager.hpp>
#include <unordered_map>
#include <chrono>
#include <map>
#include <vector>
#include "fix_me_png_icon.h"
#include "DmiMetadata.hpp"
#include <iostream>
#include <lodepng.h>
#include "SpriteSheet.hpp"

namespace service {

std::string_view get_file_extension(std::string_view name) {
  size_t dot_pos = name.find_last_of(".");
  if (dot_pos == std::string_view::npos) {
    return std::string_view();
  }
  return name.substr(dot_pos + 1);
}

std::unique_ptr<sf::Texture> load_texture(std::string_view name) {
  static thread_local std::vector<unsigned char> buffer;
  static thread_local std::vector<unsigned char> decode_buffer;
  buffer.clear();
  decode_buffer.clear();
  if(lodepng::load_file(buffer, {name.begin(), name.end()}) != 0) {
    return nullptr;
  }
  sf::Vector2u image_size;
  lodepng::State state;
  state.info_raw.bitdepth = 8;
  state.info_raw.colortype = LCT_RGBA;
  if (lodepng::decode(decode_buffer, image_size.x, image_size.y, state, buffer)) {
    return nullptr;
  }

  for (size_t i = 0; i < state.info_png.text_num; ++i) {
    if (std::string_view(state.info_png.text_keys[i]) == "Description") {
      DmiMetadata dmi_metadata(state.info_png.text_strings[i]);
    }
  }

  std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
  texture->create(image_size.x, image_size.y);
  texture->update(decode_buffer.data());
  return texture;
}

class SpriteManagerImpl : public SpriteManager {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Time> time;
  std::vector<std::unique_ptr<sf::Texture>> textures;
  std::unordered_map<std::string, int> loaded_textures;
  sf::Texture error_texture;
public:
  SpriteManagerImpl(std::shared_ptr<core::Logger>,
                    std::shared_ptr<service::Time>);
  std::string_view impl_name() const noexcept override;

  component::Sprite load_sprite(std::string_view name, std::string_view state = "") override;
  std::pair<sf::Texture*, sf::IntRect> get_texture(const component::Sprite& sprite) override;
};

static auto sprite_manager_build_after() {
  return core::after<core::LoggerFactory, service::Time>();
}

CORE_DEFINE_SERVICE(SpriteManager,
                    "service::SpriteManager",
                    sprite_manager_build_after(),
  [](core::ServiceLocator& locator) {
    return std::make_shared<SpriteManagerImpl>(
      std::move(locator.get<core::LoggerFactory>()->create_logger("service::SpriteManager")),
      std::move(locator.get<service::Time>())
    );
  }
);


//Methods
SpriteManagerImpl::SpriteManagerImpl(std::shared_ptr<core::Logger> logger,
                  std::shared_ptr<service::Time> time):
  //init
  logger(std::move(logger)),
  time(std::move(time))
{
  error_texture.loadFromMemory(MagickImage, sizeof(MagickImage));
}

std::string_view SpriteManagerImpl::impl_name() const noexcept {
  return "service::SpriteManagerImpl";
}

component::Sprite SpriteManagerImpl::load_sprite(std::string_view name, std::string_view state) {
  std::string string_name(name.begin(), name.end());
  auto it = loaded_textures.find(string_name);
  if (it != loaded_textures.end()) {
    return {it->second};
  }
  auto time_begin = std::chrono::steady_clock::now();
  std::unique_ptr<sf::Texture> texture = load_texture(name);
  if(!texture) {
    logger->warn("Не удалось загрузить {}", name);
    return {-1};
  }
  texture->setRepeated(false);
  bool mipmap_success = texture->generateMipmap();
  if (!mipmap_success) {
    logger->warn("Не удалось сгенерировать mipmap для {}", name);
  }
  textures.push_back(std::move(texture));
  int id = static_cast<int>(textures.size() - 1);
  loaded_textures[string_name] = id;
  auto time_end = std::chrono::steady_clock::now();
  std::chrono::duration<float, std::milli> elapsed_ms = time_end - time_begin;
  logger->info("Загружен {}, id {}, время {:03.2f} мс", name, id, elapsed_ms.count());
  return {id};
}

std::pair<sf::Texture*, sf::IntRect> SpriteManagerImpl::get_texture(const component::Sprite& sprite) {
  sf::Texture* texture;
  if (sprite.id < 0 || static_cast<size_t>(sprite.id) >= textures.size()) {
    //return {nullptr, {0,0,0,0}};
    texture = &error_texture;
  } else {
    texture = textures[static_cast<size_t>(sprite.id)].get();
  }
  sf::Vector2u texture_size = texture->getSize();
  return {texture, {0,0,static_cast<int>(texture_size.x), static_cast<int>(texture_size.y)}};
}

}
