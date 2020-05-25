#pragma once

#include <string_view>

namespace core {

class Service {
public:
  virtual ~Service() = default;
  virtual std::string_view impl_name() const noexcept;
};

}