#include <catch2/catch.hpp>
#include <core/service/LoggerFactory.hpp>

#include <iostream>


class ConsoleLogWriter final : public core::ILogWriter {
public:
  void write(core::LogMsg& msg) {
    std::cout << msg.time << ':'
              << '[' << msg.tag  << "]: "
              << msg.msg << '\n';
  }
};

TEST_CASE("DELETE ME","[del]" )
{
  ConsoleLogWriter log_writer;
  core::LogMsg msg;
  msg.tag = "TEST CASE";
  msg.msg = "xallo logger";
  msg.time = std::time(nullptr);
  log_writer.write(msg);
}
