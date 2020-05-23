#pragma once

#include <string_view>
#include <core/Service.hpp>

namespace core {

class Process : public Service {
public:
  virtual void exit(std::string_view reason) = 0;
  virtual void force_exit(std::string_view reason) = 0;
  virtual ~Process() = default;
};

}