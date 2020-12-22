#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>
#include <components/Sprite.hpp>
#include <services/Time.hpp>
#include <set>
#include <map>
#include <string>
#include <lodepng.h>
#include <cassert>
#include "DmiMetadata.hpp"

namespace {

  //ТУТ КОПИПАСТА ИЗ БЕЗВРЕМЕННО ПОЧИВШЕГО В НЕБЫТИЕ service::SpriteManager
  //ПЕРЕПИСАТЬ НАХУЙ ВСЕ

struct State {
  std::vector<float> delays;
  std::vector<std::vector<sf::IntRect>> dirs;
};

struct SpriteSheet {
  std::shared_ptr<sf::Texture> texture;
  std::map<std::string, State> states;
};

sf::IntRect calculate_rect(sf::Vector2i image_units, sf::Vector2i icon_size, int n) {
  int uint_y = n / image_units.x;
  int unit_x = n % image_units.x;
  sf::Vector2i unit_pos(unit_x * icon_size.x, uint_y * icon_size.y);
  return {unit_pos, icon_size};
}


class SpriteLoader final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Time> time;

  std::set<entt::entity> to_update_entities;

  entt::registry* p_registry = nullptr;
  entt::observer sprite_update_observer;
  entt::observer sprite_group_observer;

  std::map<std::string, std::unique_ptr<SpriteSheet>> sprite_sheets;
  std::vector<unsigned char> png_decode_buffer;
  std::vector<unsigned char> file_buffer;
public:
  SpriteLoader(std::shared_ptr<core::Logger>, std::shared_ptr<service::Time>);
  ~SpriteLoader();
  void init(entt::registry& registry) override;
  void update(entt::registry& registry) override;
  void update_entity(entt::registry&, entt::entity);
  const SpriteSheet* get_sprite_sheet(const std::string&);
  std::unique_ptr<SpriteSheet> load_dmi(const std::string&);

};

SpriteLoader::SpriteLoader(std::shared_ptr<core::Logger> logger, std::shared_ptr<service::Time> time):
  logger(std::move(logger)),
  time(std::move(time))
{}

SpriteLoader::~SpriteLoader() {
  if (p_registry) {
    sprite_group_observer.disconnect();
    sprite_update_observer.disconnect();
  }
}

void SpriteLoader::init(entt::registry& registry) {
  for (entt::entity entity : registry.view<component::Sprite>()) {
    to_update_entities.insert(entity);
  }

  p_registry = &registry;
  sprite_group_observer.connect(*p_registry, entt::collector.group<component::Sprite>());
  sprite_update_observer.connect(*p_registry, entt::collector.update<component::Sprite>());
}

void SpriteLoader::update(entt::registry& registry) {
  sprite_group_observer.each([this](const auto entity) {
    to_update_entities.insert(entity);
  });
  sprite_update_observer.each([this](const auto entity) {
    to_update_entities.insert(entity);
  });
  for (entt::entity entity : to_update_entities) {
    update_entity(registry, entity);
  }
  to_update_entities.clear();
}

void SpriteLoader::update_entity(entt::registry& registry, entt::entity entity) {
  auto& sprite = registry.get<component::Sprite>(entity);
  const SpriteSheet* sheet = get_sprite_sheet(sprite.icon);
  if (!sheet) {
    return;
  }
  if (!sheet->texture) {
    logger->error("У {} отсутствует текстура", sprite.state);
    return;
  }
  const State* state = nullptr;
  if (auto it = sheet->states.find(sprite.state); it != sheet->states.end()) {
    state = &(it->second);
  } else {
    logger->warn("У {} отсутствует {} стейт", sprite.icon, sprite.state);
    return;
  }
  
  size_t dir = 0; //пока всегда 0, потом добавить компонент в который указывается это
  try {
    const std::vector<sf::IntRect> frames = state->dirs.at(dir);
    const std::vector<float> delays = state->delays;

    if (frames.size() == 0) {
      throw std::runtime_error("отсутствуют фреймы");
    }

    if (frames.size() == 1) {
      registry.emplace_or_replace<component::TextureRegion>(entity, frames[0]);
    } else {
      if (frames.size() != delays.size()) {
        throw std::runtime_error("frames.size() != delays.size()");
      }
      component::TextureAutoAnimation auto_animation;
      for (size_t i = 0; i < frames.size(); ++i) {
        auto_animation.frames.push_back({delays[i], frames[i]});
        auto_animation.full_time += delays[i];
      }
      registry.emplace_or_replace<component::TextureAutoAnimation>(entity, std::move(auto_animation));
    }
  } catch(std::exception& e) {
    logger->error(
      "Icon: {}, State: {} - Ошибка выбора региона текстуры: {}", sprite.icon, sprite.state,e.what()
    );
    return;
  }
  registry.emplace_or_replace<component::Texture>(entity, sheet->texture);
}

std::unique_ptr<SpriteSheet> SpriteLoader::load_dmi(const std::string& filename) {
  png_decode_buffer.clear();
  file_buffer.clear();

  if(lodepng::load_file(file_buffer, filename) != 0) {
    return nullptr;
  }
  sf::Vector2u image_size;
  lodepng::State state;
  state.info_raw.bitdepth = 8;
  state.info_raw.colortype = LCT_RGBA;
  if (lodepng::decode(png_decode_buffer, image_size.x, image_size.y, state, file_buffer) != 0) {
    logger->error("Не удалось раскодировать {}", filename);
    return nullptr;
  }
  std::string_view png_description;
  for (size_t i = 0; i < state.info_png.text_num; ++i) {
    if (std::string_view(state.info_png.text_keys[i]) == "Description") {
      png_description = state.info_png.text_strings[i];
    }
  }
  if (png_description.size() == 0) {
    logger->error("Отсутствуют необходимые метаданные для загрузки {}", filename);
    return nullptr;
  }
  std::unique_ptr<DmiMetadata> dmi_metadata;
  try {
    dmi_metadata = std::make_unique<DmiMetadata>(png_description);
  } catch (std::exception& e) {
    logger->error("Ошибка парсинга метаданых {}: {}", filename, e.what());
    return nullptr;
  }
  std::unique_ptr sprite_sheet = std::make_unique<SpriteSheet>();
  sprite_sheet->texture = std::make_shared<sf::Texture>();
  sprite_sheet->texture->create(image_size.x, image_size.y);
  sprite_sheet->texture->update(png_decode_buffer.data());
  
  int total_images = 0;
  for(const DmiIconState& dmi_icon_state : dmi_metadata->states) {
    total_images += dmi_icon_state.dirs * dmi_icon_state.frames;
  }
  sf::Vector2i image_units(static_cast<int>(image_size.x) / dmi_metadata->width,
                           static_cast<int>(image_size.y) / dmi_metadata->height);
  if (image_units.x * image_units.y < total_images) {
    logger->error("Размер {} меньше необходимого", filename);
    return nullptr;
  }

  int current_image = 0;
  sf::Vector2i unit_size(dmi_metadata->width, dmi_metadata->height);
    for (const DmiIconState& dmi_icon_state : dmi_metadata->states) {
    State state;
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
    sprite_sheet->states.insert({dmi_icon_state.name, std::move(state)});
  }
  return sprite_sheet;
}

const SpriteSheet* SpriteLoader::get_sprite_sheet(const std::string& filename) {
  if (auto it = sprite_sheets.find(filename); it != sprite_sheets.end()) {
    return it->second.get();
  }

  float time_begin = time->get_real_time();
  std::unique_ptr<SpriteSheet> sprite_sheet = load_dmi(filename);
  float time_end = time->get_real_time();
  if (sprite_sheet) {
    logger->info("Загружен {}, время {:03.2f} мс", filename, (time_end - time_begin) * 1000.0f);
    auto [insert_it, is_inserted] = sprite_sheets.insert_or_assign(filename, std::move(sprite_sheet));
    return insert_it->second.get();
  } else {
    return nullptr;
  }
}


CORE_DEFINE_SYSTEM("system::SpriteLoader", [](core::ServiceLocator& locator){
  return std::make_unique<SpriteLoader>(
    locator.get<core::LoggerFactory>()->create_logger("system::SpriteLoader"),
    locator.get<service::Time>()
  );
});

}