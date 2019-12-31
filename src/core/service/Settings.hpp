#pragma once
#include "service_helpers.hpp"

namespace core::service {

class Settings {
public:
  static constexpr const std::string_view name = "core::service::Settings";

  int view_min_x = 7;
  int view_min_y = 7;
  int view_max_x = 14;
  int view_max_y = 14;
};

}
