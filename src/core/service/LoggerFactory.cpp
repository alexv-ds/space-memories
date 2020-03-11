#include "LoggerFactory.hpp"

#include <cstdio>
#include <iostream>
#include <termcolor/termcolor.hpp>

namespace core::service {

//На первое время и так сойдет
class ConsoleLogger final : public ILogger {
public:
  ConsoleLogger(const char* logtag): logtag(logtag) {
    static_assert(sizeof(buffer) > 0);
    buffer[sizeof(buffer) - 1] = 0;
  }
  void info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    std::cout << '[' << logtag << "] ";
    std::cout << buffer << '\n';
  }
  void warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    std::cout << '[' << logtag << ']';
    std::cout << termcolor::yellow << "[WARN] " << termcolor::reset;
    std::cout << buffer << '\n';
  }
  void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    std::cout << '[' << logtag << ']';
    std::cout << termcolor::red << "[ERROR] " << termcolor::reset;
    std::cout << buffer << '\n';
  }
  void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    std::cout << '[' << logtag << ']';
    std::cout << termcolor::cyan << "[DEBUG] " << termcolor::reset;
    std::cout << buffer << '\n';
  }
private:
  const char* const logtag;
  char buffer[256];
};

std::unique_ptr<ILogger> LoggerFactory::create_logger(const char* logtag) {
  return std::make_unique<ConsoleLogger>(logtag);
}


} /* core::service */
