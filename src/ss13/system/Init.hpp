#pragma once

#include <core/ISystem.hpp>

namespace ss13::system {

class Init final : public core::ISystem {
public:
  const std::string_view name() const noexcept override;
  void init() override;
};


} /* ss13::system */
