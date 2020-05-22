#pragma once

#include <string_view>
#include <memory>
#include <core/Service.hpp>
#include <core/Logger.hpp>

namespace core {

class LoggerFactory : public Service {
public:
  virtual std::shared_ptr<Logger> create_logger(std::string_view name) = 0; 
};

}