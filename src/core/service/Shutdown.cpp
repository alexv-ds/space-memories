#include "Shutdown.hpp"

#include <cstdlib>

namespace core::service {

void Shutdown::graceful_close() {
  this->close = true;
}

[[noreturn]] void Shutdown::force_close(int exit_code) {
  this->close = true;
  std::exit(exit_code);
}


} /* core::service */
