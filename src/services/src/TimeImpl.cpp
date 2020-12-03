#include <core/define_service.hpp>
#include <services/Time.hpp>
#include <chrono>

namespace service {

class TimeImpl final : public Time {
  using FpSecondsDuration = std::chrono::duration<float, std::chrono::seconds::period>;
public:
  TimeImpl(): start_time(std::chrono::steady_clock::now()) {}

  std::string_view impl_name() const noexcept override {
    return "service::TimeImpl";
  }

  void update() override {
    ++tick;
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    float fp_current_time = FpSecondsDuration(current_time - start_time).count();
    delta = fp_current_time - time;
    time = fp_current_time;
  }
  
  float get_real_time() const noexcept override {
    return FpSecondsDuration(std::chrono::high_resolution_clock::now() - start_time).count();
  }
  
private:
  const std::chrono::steady_clock::time_point start_time;
};

CORE_DEFINE_SERVICE(Time, "service::Time", core::after(), [](core::ServiceLocator&) {
  return std::make_shared<TimeImpl>();
});

}
