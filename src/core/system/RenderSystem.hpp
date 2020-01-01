#pragma once

#include <core/ISystem.hpp>
#include <core/service/Window.hpp>
#include <core/service/TextureLoader.hpp>

namespace core::system {

class RenderSystem final : public ISystem {
public:
  const std::string_view name() const noexcept override;
  RenderSystem();
  void update() override;
private:
  std::shared_ptr<service::Window> window;
  std::shared_ptr<service::TextureLoader> texture_loader;
};

} /* core::system */
