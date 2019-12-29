#include "Init.hpp"

#include <iostream>

namespace ss13::system {

const std::string_view Init::name() const noexcept {
  return "ss13::system::Init";
}

void Init::init() {
  std::cout << "Init::init()" << '\n';
}

} /* ss13::system */
