#pragma once

#include "service_helpers.hpp"
#include <core/RateLimiter.hpp>

namespace core::service {

class PerformanceController {
public:
  static constexpr const std::string_view name = "core::service::PerformanceController";

  inline PerformanceController(RateLimiter& rate_limiter):
    main_loop_rate_limiter(rate_limiter)
  {}

  inline float tick_usage() {
    return this->main_loop_rate_limiter.get_tick_usage_relative();
  }

private:
  RateLimiter& main_loop_rate_limiter;

};


} /* core::service */
