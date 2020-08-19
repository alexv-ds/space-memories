#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/Time.hpp>
#include <services/SpriteManager.hpp>
#include <lodepng.h>
#include <chrono>
#include <map>
#include <vector>
#include "SpriteSheet.hpp"
#include "DmiMetadata.hpp"
#include "fix_me_png_icon.h"

namespace service {

class SpriteManagerImpl : public SpriteManager {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Time> time;
  std::vector<std::unique_ptr<SpriteSheet>> sprite_sheets;
  std::map<std::string, int> loaded_sprite_sheets;
  std::vector<unsigned char> file_buffer;
  sf::Texture error_texture;
public:
  SpriteManagerImpl(std::shared_ptr<core::Logger>,
                    std::shared_ptr<service::Time>);
  std::string_view impl_name() const noexcept override;

  component::Sprite load_sprite(std::string_view name, std::string_view state = "") override;
  std::pair<sf::Texture*, sf::IntRect> get_texture(const component::Sprite& sprite) override;
  const State* get_sprite_data(int icon, int state) override;
private:
  std::unique_ptr<SpriteSheet> load_sfml(std::string_view file); //bmp, png, tga, jpg, psd, hdr and pic
  std::unique_ptr<SpriteSheet> load_dmi(std::string_view file); //dmi
  std::unique_ptr<SpriteSheet> load_sprite_sheet(std::string_view file);
  inline std::pair<sf::Texture*, sf::IntRect> get_error_texture();
  std::string_view get_file_extension(std::string_view name);
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
  auto time_begin = std::chrono::steady_clock::now();
  auto it = loaded_sprite_sheets.find({name.begin(), name.end()});
  if (it != loaded_sprite_sheets.end()) {
    if (it->second < 0) {
      return {-1, -1};
    } else {
      return {it->second, sprite_sheets[static_cast<size_t>(it->second)]->find(state)};
    }
  }
  std::unique_ptr<SpriteSheet> sprite_sheet = load_sprite_sheet(name);
  if (!sprite_sheet) {
    logger->error("Не удалось загрузить {}", name);
    loaded_sprite_sheets[{name.begin(), name.end()}] = -1;
    return {-1, -1};
  }
  component::Sprite sprite;
  sprite.state = sprite_sheet->find(state);
  sprite_sheets.push_back(std::move(sprite_sheet));
  sprite.icon = sprite_sheets.size() - 1;
  loaded_sprite_sheets[{name.begin(), name.end()}] = sprite.icon;
  auto time_end = std::chrono::steady_clock::now();
  std::chrono::duration<float, std::milli> elapsed_ms = time_end - time_begin;
  logger->info("Загружен {}, id {}, время {:03.2f} мс", name, sprite.icon, elapsed_ms.count());
  return sprite;
}

std::pair<sf::Texture*, sf::IntRect> SpriteManagerImpl::get_texture(const component::Sprite& sprite) {
  if (sprite.icon < 0 || sprite.state < 0) {
    return get_error_texture();
  }
  size_t sprite_sheet_index = static_cast<size_t>(sprite.icon);
  if (sprite_sheet_index >= sprite_sheets.size()) {
    return get_error_texture();
  }
  const std::unique_ptr<SpriteSheet>& sprite_sheet = sprite_sheets[sprite_sheet_index];
  size_t state_index = static_cast<size_t>(sprite.state);
  if (state_index >= sprite_sheet->states.size()) {
    return get_error_texture();
  }
  size_t dir_index = static_cast<size_t>(sprite.dir);
  if (dir_index >= sprite_sheet->states[state_index].dirs.size()) {
    return get_error_texture();
  }
  size_t frame_index = static_cast<size_t>(sprite.frame);
  if (frame_index >= sprite_sheet->states[state_index].dirs[dir_index].size()) {
    return get_error_texture();
  }
  return {&sprite_sheet->texture, sprite_sheet->states[state_index].dirs[dir_index][frame_index]};
}

std::unique_ptr<SpriteSheet> SpriteManagerImpl::load_sfml(std::string_view file) {
  file_buffer.clear();
  if(lodepng::load_file(file_buffer, {file.begin(), file.end()}) != 0) {
    return nullptr;
  }
  std::unique_ptr sprite_sheet = std::make_unique<SpriteSheet>();
  if(sprite_sheet->texture.loadFromMemory(file_buffer.data(), file_buffer.size()) == false) {
    return nullptr;
  }
  sf::Vector2u texture_size = sprite_sheet->texture.getSize();
  State state = {"", {}, {{{0, 0, static_cast<int>(texture_size.x), static_cast<int>(texture_size.y)}}}};
  sprite_sheet->states.push_back(std::move(state));
  return sprite_sheet;
}

std::unique_ptr<SpriteSheet> SpriteManagerImpl::load_sprite_sheet(std::string_view file) {
  std::string_view file_extension = get_file_extension(file);
  if (file_extension == "dmi") {
    return load_dmi(file);
  } else if (file_extension == "png") {
    return load_sfml(file);
  } else if (file_extension == "tga") {
    return load_sfml(file);
  } else if (file_extension == "jpg") {
    return load_sfml(file);
  } else if (file_extension == "psd") {
    return load_sfml(file);
  } else if (file_extension == "hdr") {
    return load_sfml(file);
  } else if (file_extension == "pic") {
    return load_sfml(file);
  } else if (file_extension == "bmp") {
    return load_sfml(file);
  }
  logger->error("Неизвестный формат {}", file);
  return nullptr;
}

inline std::pair<sf::Texture*, sf::IntRect> SpriteManagerImpl::get_error_texture() {
  sf::Vector2u texture_size = error_texture.getSize();
  return {&error_texture, {0,0,static_cast<int>(texture_size.x), static_cast<int>(texture_size.y)}};
}

std::string_view SpriteManagerImpl::get_file_extension(std::string_view name) {
  size_t dot_pos = name.find_last_of(".");
  if (dot_pos == std::string_view::npos) {
    return std::string_view();
  }
  return name.substr(dot_pos + 1);
}

sf::IntRect calculate_rect(sf::Vector2i image_units, sf::Vector2i icon_size, int n) {
  int uint_y = n / image_units.x;
  int unit_x = n % image_units.x;
  sf::Vector2i unit_pos(unit_x * icon_size.x, uint_y * icon_size.y);
  return {unit_pos, icon_size};
}

std::unique_ptr<SpriteSheet> SpriteManagerImpl::load_dmi(std::string_view file) {
  static thread_local std::vector<unsigned char> decode_buffer;
  decode_buffer.clear();
  file_buffer.clear();
  if(lodepng::load_file(file_buffer, {file.begin(), file.end()}) != 0) {
    return nullptr;
  }
  sf::Vector2u image_size;
  lodepng::State state;
  state.info_raw.bitdepth = 8;
  state.info_raw.colortype = LCT_RGBA;
  if (lodepng::decode(decode_buffer, image_size.x, image_size.y, state, file_buffer) != 0) {
    return nullptr;
  }
  std::string_view png_description;
  for (size_t i = 0; i < state.info_png.text_num; ++i) {
    if (std::string_view(state.info_png.text_keys[i]) == "Description") {
      png_description = state.info_png.text_strings[i];
    }
  }
  if (png_description.size() == 0) {
    logger->error("Отсутствуют необходимые метаданные для загрузки {}", file);
    return nullptr;
  }
  std::unique_ptr<DmiMetadata> dmi_metadata;
  try {
    dmi_metadata = std::make_unique<DmiMetadata>(png_description);
  } catch (std::exception& e) {
    logger->error("Ошибка парсинга метаданых {}: {}", file, e.what());
    return nullptr;
  }
  std::unique_ptr sprite_sheet = std::make_unique<SpriteSheet>();
  sprite_sheet->texture.create(image_size.x, image_size.y);
  sprite_sheet->texture.update(decode_buffer.data());

  int total_images = 0;
  for(const DmiIconState& dmi_icon_state : dmi_metadata->states) {
    total_images += dmi_icon_state.dirs * dmi_icon_state.frames;
  }
  sf::Vector2i image_units(static_cast<int>(image_size.x) / dmi_metadata->width,
                            static_cast<int>(image_size.y) / dmi_metadata->height);
  if (image_units.x * image_units.y < total_images) {
    logger->error("Размер {} меньше необходимого", file);
    return nullptr;
  }

  int current_image = 0;
  sf::Vector2i unit_size(dmi_metadata->width, dmi_metadata->height);
  for (const DmiIconState& dmi_icon_state : dmi_metadata->states) {
    State state;
    state.name = dmi_icon_state.name;
    state.delays = dmi_icon_state.delays;
    for (int dir = 1; dir <= dmi_icon_state.dirs; ++dir) {
      std::vector<sf::IntRect> vec_dir;
      for (int frame = 1; frame <= dmi_icon_state.frames; ++frame) {
        sf::IntRect rect = calculate_rect(image_units, unit_size, current_image);
        vec_dir.push_back(rect);
        ++current_image;
      }
      state.dirs.push_back(std::move(vec_dir));
    }
    for (float& delay : state.delays) {
      delay /= 10; //потому что в бьенде указано время в тиках (1 тик 1/10 секунды)
    }
    sprite_sheet->states.push_back(std::move(state));
  }
  return sprite_sheet;
}

const State* SpriteManagerImpl::get_sprite_data(int icon, int state) {
  if (icon < 0 || state < 0) {
    return nullptr;
  }
  size_t sprite_sheet_index = static_cast<size_t>(icon);
  if (sprite_sheet_index >= sprite_sheets.size()) {
    return nullptr;
  }
  const std::unique_ptr<SpriteSheet>& sprite_sheet = sprite_sheets[sprite_sheet_index];
  size_t state_index = static_cast<size_t>(state);
  if (state_index >= sprite_sheet->states.size()) {
    return nullptr;
  }
  return &(sprite_sheet->states[state_index]);
}

}
