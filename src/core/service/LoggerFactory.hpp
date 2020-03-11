#pragma once

#include <core/IService.hpp>
#include <core/ILogger.hpp>
#include <memory>

namespace core::service {

class LoggerFactory : public IService {
public:
  std::unique_ptr<ILogger> create_logger(const char* logtag);
};

} /* core::service */
