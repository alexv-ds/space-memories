#include <core/define_system.hpp>
#include <core/Process.hpp>
#include <core/LoggerFactory.hpp>
#include <ctime>

using namespace std::chrono_literals;

class ExitAfter10sec final : public core::System {
  std::shared_ptr<core::Logger> logger;
  std::shared_ptr<core::Process> process;

  std::time_t seconds = 10;
  std::time_t last_time;
  
  
public:
  ExitAfter10sec(std::shared_ptr<core::Logger> logger, std::shared_ptr<core::Process> process) {
    this->logger = logger;
    this->process = process;

    std::time(&last_time);
  }
  void update() override {

    if (seconds == 0) {
      return;
    }
    std::time_t current_time;
    std::time(&current_time);

    if ((current_time - last_time) == 1) {
      last_time = current_time;
      --seconds;
      if (seconds == 0) {
        process->exit("Потому что могу");
        return;
      }
      logger->info("Осталось {} секунд", seconds);      
    }

  };
};

CORE_DEFINE_SYSTEM("system::ExitAfter10sec", [](core::ServiceLocator& locator){
  std::shared_ptr logger_factory = locator.get<core::LoggerFactory>();
  std::shared_ptr process = locator.get<core::Process>();
  return std::make_unique<ExitAfter10sec>(
    std::move(logger_factory->create_logger("ExitAfter10sec")),
    std::move(process)
  );
});

