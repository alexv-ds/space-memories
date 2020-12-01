#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <core/Service.hpp>

namespace core {

class SystemManager : public Service {
public:
  struct SystemStatus;
  
  virtual void each_system(std::function<void(const SystemStatus&)>) = 0;
  virtual bool toggle_system(std::string_view) = 0;
};

struct SystemManager::SystemStatus {
  std::string name;
  int priority;
  bool active;
};

}