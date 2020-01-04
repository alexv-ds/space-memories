#pragma once

#include <core/IService.hpp>
#include <memory>
#include <ctime>
#include <string_view>

namespace core {

class ILogWriter {
public:
  bool log_info = true;
  bool log_warn = true;
  bool log_critical = true;
  bool log_debug = true;
  virtual ~ILogWriter() = default;
  //virtual void write() = 0;
};

enum class LogType {
  info,
  warn,
  critical,
  debug
};

struct LogMsg {
  std::string_view tag;
  std::string_view msg;
  std::time_t time;
  LogType type;
};

class Logger {
public:
  void info(const char* format, ...);
  void warn(const char* format, ...);
  void critical(const char* format, ...);
  void debug(const char* format, ...);
private:
  void log(LogType type, const char* format, ...);
};

} /* core */



namespace core::service {



class LoggerFactory : public IService {
public:

  std::shared_ptr<Logger> create_logger(const char* logtag);
  void register_log_writer(std::shared_ptr<ILogWriter>);
};


} /* core::service */
