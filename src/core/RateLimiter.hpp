#pragma once

#include <chrono>

namespace core {

class RateLimiter {
public:
  inline RateLimiter(const float fps) noexcept;
  inline void set_fps(const float fps) noexcept;
  inline float get_fps() const noexcept;
  float get_tick_usage_relative() const noexcept;
  void wait_next() noexcept; //блокирующий вызов

private:
  using time_duration = std::chrono::duration<float>;
  std::chrono::steady_clock::time_point start_time;
  time_duration tick_time_quota = time_duration::zero();
};

//////////////////
//Inline methods//
//////////////////
inline RateLimiter::RateLimiter(const float fps) noexcept {
  set_fps(fps);
}

inline void RateLimiter::set_fps(const float fps) noexcept {
  this->tick_time_quota = time_duration(1.0f / fps);
}

inline float RateLimiter::get_fps() const noexcept {
  return 1.0f / tick_time_quota.count();
}


} /* core */
