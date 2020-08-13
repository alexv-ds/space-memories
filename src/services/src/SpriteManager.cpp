#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/Time.hpp>
#include <services/SpriteManager.hpp>
#include <unordered_map>
#include <chrono>

namespace service {

class SpriteManagerImpl : public SpriteManager {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<service::Time> time;
  std::vector<std::unique_ptr<sf::Texture>> textures;
  std::unordered_map<std::string, int> loaded_textures;
public:
  SpriteManagerImpl(std::shared_ptr<core::Logger> logger,
                    std::shared_ptr<service::Time> time):
    //init
    logger(std::move(logger)),
    time(std::move(time))
  {}

  std::string_view impl_name() const noexcept override {
    return "service::SpriteManagerImpl";
  }

  int load_sprite(std::string_view name, std::string_view state = "") override {
    std::string string_name(name.begin(), name.end());
    auto it = loaded_textures.find(string_name);
    if (it != loaded_textures.end()) {
      return it->second;
    }
    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    auto time_begin = std::chrono::steady_clock::now();
    bool load_success = texture->loadFromFile({name.begin(), name.end()});
    if(!load_success) {
      logger->warn("Не удалось загрузить {}", name);
      return -1;
    }
    texture->setRepeated(false);
    bool mipmap_success = texture->generateMipmap();
    if (!mipmap_success) {
      logger->warn("Не удалось сгенерировать mipmap для {}", name);
    }
    //texture->setSmooth(true);
    textures.push_back(std::move(texture));
    int id = static_cast<int>(textures.size() - 1);
    loaded_textures[string_name] = id;
    auto time_end = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::milli> elapsed_ms = time_end - time_begin;
    logger->info("Загружен {}, id {}, время {:03.2f} мс", name, id, elapsed_ms.count());
    return id;
  }

  sf::Texture* get_texture(int id) override {
    if (id < 0 || static_cast<size_t>(id) >= textures.size()) {
      return nullptr;
    }
    return textures[static_cast<size_t>(id)].get();
  }
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
  });
}
