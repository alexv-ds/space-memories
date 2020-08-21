#pragma once
#include <string>

namespace component {

//C++ only
struct ExitAfterNsec {
  std::string reason = "no reason";
  int delay = 0;
};

}
