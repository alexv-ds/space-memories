#pragma once

namespace core {

class ILogger {
public:
  virtual void info(const char* format, ...) = 0;
  virtual void warn(const char* format, ...) = 0;
  virtual void error(const char* format, ...) = 0;
  virtual void debug(const char* format, ...) = 0;
  virtual ~ILogger() = default;
};

} /* core */
