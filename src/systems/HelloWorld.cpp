#include <core/define_system.hpp>
#include <core/LoggerFactory.hpp>

class HelloWorld final : public core::System {
  std::shared_ptr<core::Logger> logger;
  bool sended = false;
public:
  HelloWorld(std::shared_ptr<core::Logger> logger): logger(logger) {}
  void update() override {
    if(sended) {
      return;
    }
    sended = true;
    logger->critical("Hello World");
  }
};

CORE_DEFINE_SYSTEM("system::HelloWorld", [](core::ServiceLocator& locator){
  return std::make_unique<HelloWorld>(
    std::move(locator.get<core::LoggerFactory>()->create_logger("system::HelloWorld"))
  );
});