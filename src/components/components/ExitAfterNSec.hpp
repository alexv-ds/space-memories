#pragma once
#include <string>

namespace component {

struct ExitAfterNsec {
  std::string reason = "no reason";
  int delay = 0;
};

}
