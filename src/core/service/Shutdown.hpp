#pragma once

#include "service_helpers.hpp"

namespace core::service {

class Shutdown {
public:
  static constexpr const std::string_view name = "core::service::Shutdown";
  void graceful_close();
  [[noreturn]] void force_close(int exit_code = 0);
  inline bool on_close() const noexcept;
private:
  bool close = false;
};

//////////////////
//Inline Methods//
//////////////////
bool Shutdown::on_close() const noexcept {
  return this->close;
}

} /* core::service */
