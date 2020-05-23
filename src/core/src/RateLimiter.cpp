#include "RateLimiter.hpp"

#include <thread>

namespace core {

void RateLimiter::wait_next() noexcept {
  using namespace std::chrono;
  steady_clock::time_point now_time = steady_clock::now();
  time_duration tick_usage = duration_cast<time_duration>(now_time - this->start_time);
  if (tick_usage < this->tick_time_quota) {
    std::this_thread::sleep_for(this->tick_time_quota - tick_usage);
    this->start_time = steady_clock::now();
  } else {
    this->start_time = now_time;
  }
}

float RateLimiter::get_tick_usage_relative() const noexcept {
  using namespace std::chrono;
  time_duration tick_usage = duration_cast<time_duration>(
    steady_clock::now() - this->start_time
  );
  return tick_usage / this->tick_time_quota;
}

} /* core */