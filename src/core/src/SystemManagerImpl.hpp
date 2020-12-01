#pragma once

#include <core/Service.hpp>
#include <core/SystemManager.hpp>
#include <core/Logger.hpp>
#include "SystemRegistry.hpp"

namespace core {

class SystemManagerImpl : public SystemManager {
public:
  SystemManagerImpl(std::shared_ptr<Logger>,
                    std::shared_ptr<SystemRegistry>);
  std::string_view impl_name() const noexcept override;
  
  void each_system(std::function<void(const SystemStatus&)>) override;
  virtual bool toggle_system(std::string_view) override;
  
private:
  std::shared_ptr<Logger> logger;
  std::shared_ptr<SystemRegistry> system_registry;
  SystemStatus system_status_cache;
};

}